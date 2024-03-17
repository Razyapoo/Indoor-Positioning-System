#include "videoprocessor.h"

VideoProcessor::VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor):
    frameQueue(frameQueue)
    , dataProcessor(dataProcessor)
    , isPaused(false)
    , shouldStopVideoProcessing(false)
    , isExportRequested(false)
{

    videoProcessorThread.reset(new QThread);
    moveToThread(videoProcessorThread.get());

    // connect(videoProcessorThread.get(), &QThread::started, this, &VideoProcessor::processVideo);
    connect(this, &VideoProcessor::requestFindUWBMeasurementAndEnqueue, dataProcessor, &DataProcessor::onFindUWBMeasurementAndEnqueue, Qt::BlockingQueuedConnection);
    connect(this, &VideoProcessor::requestFindUWBMeasurementAndExport, dataProcessor, &DataProcessor::onFindUWBMeasurementAndExport, Qt::BlockingQueuedConnection);

    // loop.exec();
    videoProcessorThread->start();
}

VideoProcessor::~VideoProcessor() {
    QMetaObject::invokeMethod(this, "cleanup");
    // cleanup();
    videoProcessorThread->wait();
}

void VideoProcessor::init(const std::string& filename) {
    pauseProcessing();
    {
        QMutexLocker locker(&mutex);
        if (camera.isOpened()) {
            camera.release();
        }
        if (!camera.open(filename)) {
            // emit error("Failed to open video file.");
            std::cout << "Failed to open video file." << std::endl;
            return;
        }
        humanDetector.initHumanDetection("/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/weights/yolov4.cfg", "/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/weights/yolov4.weights");
    }

    totalFrames = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_COUNT));
    fps = camera.get(cv::CAP_PROP_FPS);
    videoDuration = totalFrames / fps;
    resumeProcessing();
    // processVideo();
}

void VideoProcessor::cleanup() {
    if (camera.isOpened()) {
        camera.release();
    }
    // loop.quit();
    videoProcessorThread->quit();
}

double VideoProcessor::getVideoDuration() const {
    return videoDuration;
}

double VideoProcessor::getFPS() const {
    return fps;
}

int VideoProcessor::getTotalFrames() const {
    return totalFrames;
}

void VideoProcessor::processVideo() {


    while (!shouldStopVideoProcessing) {

        if (shouldStopVideoProcessing) break;


        {
            QMutexLocker locker(&mutex);
            while (isPaused && !isSeekRequested && !isExportRequested) {
                pauseCondition.wait(&mutex);
            }
        }

        if (isSeekRequested) {
            {
                QMutexLocker locker(&mutex);
                camera.set(cv::CAP_PROP_POS_FRAMES, seekPosition - 1); // -1 because of the following read
                isSeekRequested = false;
                emit seekingDone();
            }
        }

        {
            QMutexLocker locker(&mutex);
            if (!camera.read(frame)) {
                pause(); // in case it will be necessary to read again
                continue;
            }
        }


        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        int position;
        {
            QMutexLocker locker(&mutex);
            position = static_cast<int>(camera.get(cv::CAP_PROP_POS_FRAMES));
        }

        if (isExportRequested) {
            {
                QMutexLocker locker(&mutex);
                if (position > endDataExportPosition) {
                    std::cout << "Position is out of desired range" << std::endl;
                    shouldStopExport = true;
                } else {
                    while (position <= endDataExportPosition) {
                        if (shouldStopExport) {
                            break;
                        }
                        std::vector<QPoint> bottomEdgeCentersVector = detectPeople(frame);
                        if (position != endDataExportPosition) {
                            emit requestFindUWBMeasurementAndExport(position, bottomEdgeCentersVector, false);
                        } else {
                            emit requestFindUWBMeasurementAndExport(position, bottomEdgeCentersVector, true);
                        }
                        ++position;
                    }
                }
            }
            isPaused = true;
            isExportRequested = false;
            if (shouldStopExport) {
                shouldStopExport = false;
                emit exportFinished(false);
            } else {
                emit exportFinished(true);
            }
            continue;
        } else {


            // if (detectPeople)
            // {

            // // }

            // // frame = detectionImage;


            if (qImage.isNull() || qImage.width() != frame.cols || qImage.height() != frame.rows) {
                qImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            }

            if (frame.data != qImage.bits()) {
                memcpy(qImage.bits(), frame.data, static_cast<size_t>(frame.cols * frame.rows * frame.channels()));
            }

            // Decided to go with copy constructor and not move. Otherwise there is a need to sync videoProcessor and dataProcessor - may introduce difficulties in communication.
            emit requestFindUWBMeasurementAndEnqueue(position, qImage);
        }
    }

    camera.release();
}

std::vector<QPoint> VideoProcessor::detectPeople(cv::Mat& frame) {

    int idx;
    cv::Rect box;
    std::vector<QPoint> bottomEdgeCentersVector;
    std::pair<std::vector<cv::Rect>, std::vector<int>> detectedPeople = humanDetector.detectPeople(frame);
    if (!detectedPeople.first.empty() && !detectedPeople.second.empty())
    {
        for (int i = 0; i < detectedPeople.second.size(); i++)
        {
            QPoint bottomEdgeCenter;
            idx = detectedPeople.second[i];
            box = detectedPeople.first[idx];
            bottomEdgeCenter.setX(box.x + (box.width / 2));
            bottomEdgeCenter.setY(box.y + box.height);
            bottomEdgeCentersVector.push_back(bottomEdgeCenter);

            cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 2);
        }
    }

    cv::resize(frame, frame, cv::Size(640, 360));
    return bottomEdgeCentersVector;
}

void VideoProcessor::resumeProcessing() {
    isPaused = false;
    QMutexLocker locker(&mutex);
    pauseCondition.wakeOne();
}

void VideoProcessor::pauseProcessing() {
    // QMutexLocker locker(&mutex);
    isPaused = true;
    // emit processingIsPaused();
}

void VideoProcessor::stopProcessing() {
    // QMutexLocker locker(&mutex);
    shouldStopVideoProcessing = true;
    resumeProcessing();
}

void VideoProcessor::seekToFrame(int position) {
    // isPaused = false;
    isSeekRequested = true;

    QMutexLocker locker(&mutex);
    seekPosition = position;
    pauseCondition.wakeOne();
}

void VideoProcessor::dataExport(int endPosition) {
    isExportRequested = true;

    QMutexLocker locker(&mutex);
    endDataExportPosition = endPosition;
    // isPaused = false;
    // pauseCondition.wakeOne();
}

void VideoProcessor::stopExport() {
    // QMutexLocker locker(&mutex);
    shouldStopExport = true;
}


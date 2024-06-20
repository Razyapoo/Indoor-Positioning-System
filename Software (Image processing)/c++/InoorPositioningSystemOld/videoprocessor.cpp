#include "videoprocessor.h"

VideoProcessor::VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor):
    frameQueue(frameQueue)
    , dataProcessor(dataProcessor)
    , isPaused(false)
    , shouldStopVideoProcessing(false)
    , isExportRequested(false)
    , humanDetector(HumanDetector())
{

    videoProcessorThread.reset(new QThread);
    moveToThread(videoProcessorThread.get());

    detectionFrameSize = cv::Size(640, 640);

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
        // humanDetector.initHumanDetection("/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/weights/yolov4.cfg", "/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/weights/yolov4.weights");
    }

    totalFrames = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_COUNT));
    fps = camera.get(cv::CAP_PROP_FPS);
    videoDuration = totalFrames / fps;
    isDistCoeffSet = false;
    resumeProcessing();
}

void VideoProcessor::cleanup() {
    if (camera.isOpened()) {
        camera.release();
    }

    videoProcessorThread->quit();
}

void VideoProcessor::initHumanDetector(const std::string &modelConfiguration, const std::string &modelWeights) {
    humanDetector.initHumanDetection(modelConfiguration, modelWeights);
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
                camera.set(cv::CAP_PROP_POS_FRAMES, seekPosition - 1); // -1 because of the following read. Following read saves one futher read
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

            if (cameraFrameSize.empty()) {
                cameraFrameSize = frame.size();
            }

            if (!distCoeffs.empty()) {
                if (!isDistCoeffSet) {
                    emit distCoeffLoaded();
                    isDistCoeffSet = true;
                }
                cv::Mat tempFrame;
                // std::cout << "Frame connent before undistortion: " << frame;
                // cv::undistort(frame, tempFrame, cameraMatrix, distCoeffs, optimalCameraMatrix);
                // Initialize the undistortion and rectification maps
                cv::Mat map1, map2;
                cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), optimalCameraMatrix, frame.size(), CV_16SC2, map1, map2);

                // Undistort the image using remap
                // cv::Mat dst;
                cv::remap(frame, tempFrame, map1, map2, cv::INTER_LINEAR);
                frame = tempFrame;
                // std::cout << "frame connent afetr: " << frame;
            }
        }


        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        int position;
        {
            QMutexLocker locker(&mutex);
            position = static_cast<int>(camera.get(cv::CAP_PROP_POS_FRAMES));
        }

        // export is done using frame ID, not timestamp
        if (isExportRequested && frameRangeToExport.size() > 0) {
            {
                QMutexLocker locker(&mutex);
                int i = 0;
                for (i = 0; i < frameRangeToExport.size(); ++i) {
                // for (frameID = frameRangeToExport.begin(); frameID != frameRangeToExport.end(); ++frameID) {
                    if (shouldStopExport) {
                        break;
                    }
                    camera.set(cv::CAP_PROP_POS_FRAMES, frameRangeToExport[i]);
                    if (!camera.read(frame)) {
                        std::cout << "Failed to read frame while export" << std::endl;
                        break;
                    }

                    if (!humanDetector.isInitialized()) {
                        emit humanDetectorNotInitialized();
                        shouldStopExport = true;
                        break;
                    }

                    std::vector<DetectionResult> detectionResults;
                    detectPeople(frame, detectionResults);
                    DetectionData detectionData(detectionResults, cameraFrameSize, detectionFrameSize);

                    if (i != (frameRangeToExport.size() - 1)) {
                        emit requestFindUWBMeasurementAndExport(frameRangeToExport[i], i, exportType, detectionData, false);
                    } else {
                        emit requestFindUWBMeasurementAndExport(frameRangeToExport[i], i, exportType, detectionData, true);
                        break;
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

            std::vector<DetectionResult> detectionResults;

            if (isPredictionRequested)
            {    
                detectPeople(frame, detectionResults);
            }

            DetectionData detectionData(detectionResults, cameraFrameSize, detectionFrameSize);


            if (qImage.isNull() || qImage.width() != frame.cols || qImage.height() != frame.rows) {
                qImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            }

            if (frame.data != qImage.bits()) {
                memcpy(qImage.bits(), frame.data, static_cast<size_t>(frame.cols * frame.rows * frame.channels()));
            }

            // Decided to go with copy constructor and not move. Otherwise there is a need to sync videoProcessor and dataProcessor - may introduce difficulties in communication.
            emit requestFindUWBMeasurementAndEnqueue(position, qImage, detectionData);
        }
    }

    camera.release();
}

void VideoProcessor::detectPeople(cv::Mat& frame, std::vector<DetectionResult>& detectionsVector) {

    int idx;
    cv::resize(frame, frame, detectionFrameSize);
    std::pair<std::vector<cv::Rect>, std::vector<int>> detectedPeople = humanDetector.detectPeople(frame, detectionFrameSize);
    if (!detectedPeople.first.empty() && !detectedPeople.second.empty())
    {
        for (int i = 0; i < detectedPeople.second.size(); i++)
        {
            QPoint bottomEdgeCenter;
            cv::Rect bbox;
            idx = detectedPeople.second[i];
            bbox = detectedPeople.first[idx];
            cv::rectangle(frame, bbox, cv::Scalar(0, 0, 255), 2);
            bottomEdgeCenter.setX(bbox.x + (bbox.width / 2));
            bottomEdgeCenter.setY(bbox.y + bbox.height);
            DetectionResult detectionResult = DetectionResult(std::move(bottomEdgeCenter), std::move(bbox));
            detectionsVector.push_back(std::move(detectionResult));
            // detectionsVector.push_back(std::move(std::make_pair(bottomEdgeCenter, box)));
        }
    }

    cv::resize(frame, frame, cameraFrameSize);
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

void VideoProcessor::setFrameRangeToExport(const std::vector<int>& frameRange, ExportType type) {
    isExportRequested = true;

    QMutexLocker locker(&mutex);
    frameRangeToExport = frameRange;
    exportType = type;
    // frameByFrameExportEndPosition = endPosition;
}

void VideoProcessor::stopExport() {
    shouldStopExport = true;
}

// Both predictions at the same time are possilbe. Optimized to detect people only once.
int VideoProcessor::setPredict(bool toPredict) {

    if (!humanDetector.isInitialized() && toPredict) {
        // emit humanDetectorNotInitialized();
        return -1;
    }

    isPredictionRequested = toPredict;

    return 0;
}

void VideoProcessor::setCameraMatrix(const cv::Mat& matrix) {
    // cameraMatrix = cv::Mat(3, 3, CV_64F);
    // cameraMatrix = cv::Mat(matrix).reshape(1, 3);
    // std::cout << "cameraMatrix:\n" << cameraMatrix << std::endl;
    cameraMatrix = matrix;
}

void VideoProcessor::setOptimalCameraMatrix(const cv::Mat& matrix) {
    // optimalCameraMatrix = cv::Mat(3, 3, CV_64F);
    // optimalCameraMatrix = cv::Mat(matrix).reshape(1, 3);
    // std::cout << "optimalCameraMatrix:\n" << optimalCameraMatrix.at<double>(0, 0) << std::endl;
    // std::cout << "optimalCameraMatrix:\n" << optimalCameraMatrix.at<double>(0, 1) << std::endl;
    optimalCameraMatrix = matrix;
}

void VideoProcessor::setDistCoeffs(const cv::Mat& matrix) {
    // distCoeffs.create(1, 5, CV_64F);
    // for (int i = 0; i < matrix.size(); ++i) {
    //     distCoeffs.at<double>(0, i) = matrix[i];
    // }
    distCoeffs = matrix;
    // std::cout << "dist: " << distCoeffs << std::endl;
    // std::cout << "1st: " << distCoeffs.at<double>(0, 1) << std::endl;
}

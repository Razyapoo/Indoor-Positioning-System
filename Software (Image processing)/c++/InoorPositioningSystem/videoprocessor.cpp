#include "videoprocessor.h"

VideoProcessor::VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor): frameQueue(frameQueue), dataProcessor(dataProcessor), keepProcessingVideo(true) {}
VideoProcessor::~VideoProcessor() {
    keepProcessingVideo = false;

    // if (camera.isOpened()) {
    //     camera.release();
    // }
}

void VideoProcessor::init(const QString& filename) {
    this->filename = filename.toStdString();
    camera.open(this->filename);

    if (!camera.isOpened()) {
        emit finished();
        return;
    }

    totalFrames = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_COUNT));
    fps = camera.get(cv::CAP_PROP_FPS);
    videoDuration = totalFrames / fps;

    humanDetector.initHumanDetection("/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/weights/yolov4.cfg", "/home/oskar/Documents/Master Thesis/Software (Image processing)/c++/weights/yolov4.weights");
    isExportRequested = false;
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
    while (this->isRunning()) {


        // if (!camera.isOpened()) {
        //     std::cout << "VideoStream is closed" << std::endl;
        // } else {
        //     std::cout << "VideoStream is ok" << std::endl;
        // }
        if (!camera.read(frame)) {
            emit finished();
            return;
        }

        if (!this->isRunning()) {
            break;
        }

        // qDebug() << "In video reading loop...";
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        // if (detectPeople)
        // {

        // // }

        // // frame = detectionImage;



        int position = static_cast<int>(camera.get(cv::CAP_PROP_POS_FRAMES));

        if (isExportRequested) {
            if (position > endDataExportPosition) {
                std::cout << "Position is out of range" << std::endl;
                emit exportFinished();
            } else {
                while (position <= endDataExportPosition) {
                    std::vector<QPoint> bottomEdgeCentersVector = detectPeople(frame);
                    if (position != endDataExportPosition) {
                        emit requestFindUWBMeasurementAndExport(position, bottomEdgeCentersVector, false);
                    } else {
                        emit requestFindUWBMeasurementAndExport(position, bottomEdgeCentersVector, true);
                    }
                    ++position;
                }

                if (position > endDataExportPosition) {
                    keepProcessingVideo = false;
                    isExportRequested = false;
                    emit exportFinished();
                }
            }


        } else {
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

void VideoProcessor::continueProcessing() {
    keepProcessingVideo = true;
}

void VideoProcessor::stopProcessing() {
    keepProcessingVideo = false;
    emit processingIsStopped();
}

bool VideoProcessor::isRunning() {
    return keepProcessingVideo;
}

// void VideoProcessor::seekToFrame(const double& setTimeInSeconds) {
//     // int keyframePosition = static_cast<int>((position / videoDuration) * totalFrames);
//     // keyframePosition = std::max(0, std::min(keyframePosition, totalFrames - 1));
//     // if (setTimeInSeconds < videoDuration) {
//     //     camera.set(cv::CAP_PROP_POS_MSEC, setTimeInSeconds * 1000);
//     // }

// }

void VideoProcessor::seekToFrame(int position) {
    if (position < 0 || position >= totalFrames) {
        std::cout << "Non-existing position" << std::endl;
        position = std::max(0, std::min(position, totalFrames - 1));
    }
    camera.set(cv::CAP_PROP_POS_FRAMES, position);

    continueProcessing();

    emit seekingDone();
}

void VideoProcessor::onDataExport(int endPosition) {
    isExportRequested = true;
    endDataExportPosition = endPosition;
}



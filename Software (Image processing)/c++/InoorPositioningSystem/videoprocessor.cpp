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

        if (qImage.isNull() || qImage.width() != frame.cols || qImage.height() != frame.rows) {
            qImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        }

        if (frame.data != qImage.bits()) {
            memcpy(qImage.bits(), frame.data, static_cast<size_t>(frame.cols * frame.rows * frame.channels()));
        }

        int position = static_cast<int>(camera.get(cv::CAP_PROP_POS_FRAMES));

        qDebug() << "Position: " << position;

        // Decided to go with copy constructor and not move. Otherwise there is a need to sync videoProcessor and dataProcessor - may introduce difficulties in communication.
        emit requestFindUWBMeasurement(position, qImage);
        // frameQueue.enqueue(position, qImage);
    }
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
    // double frameNumber = position / (videoDuration * fps);
    if (position < 0 || position >= totalFrames) {
        std::cout << "Non-existing position" << std::endl;
        position = std::max(0, std::min(position, totalFrames - 1));
    }
    // int frameNumber = int((1 / fps) * 1000 * position);
    // std::cout << "Frame Number: " << frameNumber << std::endl;
    frameQueue.clear();
    camera.set(cv::CAP_PROP_POS_FRAMES, position);
    // int success = camera.set(cv::CAP_PROP_POS_MSEC, frameNumber);



    // std::cout << "Success: " << success << std::endl;
    // camera.grab();
    // camera.retrieve(frame);
    // int newposition = int(camera.get(cv::CAP_PROP_POS_FRAMES));
    // camera.read(frame);
    this->continueProcessing();

    emit seekingDone();

}

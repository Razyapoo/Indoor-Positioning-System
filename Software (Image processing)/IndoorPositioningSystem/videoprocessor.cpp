#include "videoprocessor.h"

VideoProcessor::VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor):
    frameQueue(frameQueue)
    , dataProcessor(dataProcessor)
    , isPaused(false)
    , shouldStopVideoProcessing(false)
    , isExportRequested(false)
    , humanDetector(HumanDetector())
{
    // Thread initiation
    videoProcessorThread.reset(new QThread);
    moveToThread(videoProcessorThread.get());

    detectionFrameSize = cv::Size(640, 640);

    connect(this, &VideoProcessor::requestFindUWBMeasurementAndEnqueue, dataProcessor, &DataProcessor::onFindUWBMeasurementAndEnqueue, Qt::BlockingQueuedConnection);
    connect(this, &VideoProcessor::requestFindUWBMeasurementAndExport, dataProcessor, &DataProcessor::onFindUWBMeasurementAndExport, Qt::BlockingQueuedConnection);

    videoProcessorThread->start();
}

// cleanup
VideoProcessor::~VideoProcessor() {
    QMetaObject::invokeMethod(this, "cleanup");
    videoProcessorThread->wait();
}

void VideoProcessor::cleanup() {
    if (camera.isOpened()) {
        camera.release();
    }

    videoProcessorThread->quit();
}

//-------------------------------- Load necessary files for video processing --------------------------------
// Load video into cv::VideoCapture for further processing
void VideoProcessor::init(const std::string& filename) {
    pauseProcessing();
    {
        QMutexLocker locker(&mutex);
        if (camera.isOpened()) {
            camera.release();
        }
        if (!camera.open(filename)) {
            return;
        }
    }

    // set video attributes
    totalFrames = static_cast<int>(camera.get(cv::CAP_PROP_FRAME_COUNT));
    fps = camera.get(cv::CAP_PROP_FPS);
    videoDuration = totalFrames / fps;
    isDistCoeffSet = false; // Distortion coefficients for frame undistortion
    resumeProcessing();
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

//-------------------------------- Main function for video processing --------------------------------
void VideoProcessor::processVideo() {


    while (!shouldStopVideoProcessing) {

        if (shouldStopVideoProcessing) break;

        // Mutex is necessary for safe thread access from outside. It pauses thread, and force the thread to wait until "play" is pressed
        {
            QMutexLocker locker(&mutex);
            while (isPaused && !isSeekRequested && !isExportRequested) { // when exporting or seeking the frame, video processor should work properly
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
            if (!camera.read(frame) || frame.empty()) {
                camera.set(cv::CAP_PROP_POS_FRAMES, 0); // Play a video from the beginning if it was finished
                continue;
            }

            if (cameraFrameSize.empty()) { // nesessary for frame reconstruction after detecting people
                cameraFrameSize = frame.size();
            }

            // Apply undistortion if the intrinsic parameters are loaded succesfully
            if (!distCoeffs.empty()) {
                if (!isDistCoeffSet) {
                    emit distCoeffLoaded();
                    isDistCoeffSet = true;
                }
                cv::Mat tempFrame;
                // Initialize the undistortion and rectification maps
                cv::Mat map1, map2;
                cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), optimalCameraMatrix, frame.size(), CV_16SC2, map1, map2);

                // Undistort the image using remap
                cv::remap(frame, tempFrame, map1, map2, cv::INTER_LINEAR);
                frame = tempFrame;
            }
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // necessary for qImage
        int position;
        {
            QMutexLocker locker(&mutex);
            position = static_cast<int>(camera.get(cv::CAP_PROP_POS_FRAMES)); // get video position for Video Player
        }

        // Data export uses frame ID, not timestamp
        if (isExportRequested && frameRangeToExport.size() > 0) {
            {
                QMutexLocker locker(&mutex);
                int i = 0;
                for (i = 0; i < frameRangeToExport.size(); ++i) {
                    if (shouldStopExport) { // handle export interruption
                        break;
                    }
                    camera.set(cv::CAP_PROP_POS_FRAMES, frameRangeToExport[i]);
                    if (!camera.read(frame)) {
                        // std::cout << "Failed to read frame while export" << std::endl;
                        break;
                    }

                    // Safty check. Export alway involves the use of Human Detector
                    if (!humanDetector.isInitialized()) {
                        emit humanDetectorNotInitialized();
                        shouldStopExport = true;
                        break;
                    }

                    // Detect people
                    std::vector<DetectionResult> detectionResults;
                    detectPeople(frame, detectionResults);
                    DetectionData detectedPeople(detectionResults, cameraFrameSize, detectionFrameSize);

                    // Send frame with detections to Data Processor for synchronization with coordinates and export
                    if (i != (frameRangeToExport.size() - 1)) {
                        emit requestFindUWBMeasurementAndExport(frameRangeToExport[i], i, exportType, detectedPeople, false);
                    } else {
                        emit requestFindUWBMeasurementAndExport(frameRangeToExport[i], i, exportType, detectedPeople, true);
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
        } else { // If no export requested, process as usual

            std::vector<DetectionResult> detectionResults;

            // People are detected only if prediction is requested.
            // Optimized to predict only once even if both Pixel-to-Real and Optical methods are requested
            if (isPredictionRequested)
            {    
                detectPeople(frame, detectionResults);
            }
            DetectionData detectedPeople(detectionResults, cameraFrameSize, detectionFrameSize);


            // Prepare frame for Video Player. Video Player works with QImage.
            if (qImage.isNull() || qImage.width() != frame.cols || qImage.height() != frame.rows) {
                qImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            }

            if (frame.data != qImage.bits()) {
                memcpy(qImage.bits(), frame.data, static_cast<size_t>(frame.cols * frame.rows * frame.channels()));
            }

            // Send the data to DataProcessor for synchronization
            emit requestFindUWBMeasurementAndEnqueue(position, qImage, detectedPeople);
        }
    }

    camera.release();
}

//---------------- Helping functions for Play / Pause / Seeking video in Video Player -----------------------
void VideoProcessor::resumeProcessing() {
    isPaused = false;
    QMutexLocker locker(&mutex);
    pauseCondition.wakeOne();
}

void VideoProcessor::pauseProcessing() {
    isPaused = true;
}

void VideoProcessor::stopProcessing() {
    shouldStopVideoProcessing = true;
    resumeProcessing();
}

void VideoProcessor::seekToFrame(int position) {
    isSeekRequested = true;

    QMutexLocker locker(&mutex);
    seekPosition = position;
    pauseCondition.wakeOne();
}

//---------------- Helping functions to handle export -----------------------

void VideoProcessor::setFrameRangeToExport(const std::vector<int>& frameRange, ExportType type) {
    isExportRequested = true;

    QMutexLocker locker(&mutex);
    frameRangeToExport = frameRange;
    exportType = type;
}

void VideoProcessor::stopExport() {
    shouldStopExport = true;
}

//---------------- Detect people -----------------------

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

//---------------- Handle Pixel-to-Real and Optical methods -----------------------

// Both predictions at the same time are possilbe. Optimized to detect people only once.
int VideoProcessor::setPredict(bool toPredict) {

    // Safety check if Human Detector is initialized
    if (!humanDetector.isInitialized() && toPredict) {
        return -1;
    }
    isPredictionRequested = toPredict;

    // everythong is ok
    return 0;
}

void VideoProcessor::setCameraMatrix(const cv::Mat& matrix) {
    cameraMatrix = matrix;
}

void VideoProcessor::setOptimalCameraMatrix(const cv::Mat& matrix) {
    optimalCameraMatrix = matrix;
}

void VideoProcessor::setDistCoeffs(const cv::Mat& matrix) {
    distCoeffs = matrix;
}
// -------------------------------------------- End of Video Processor -------------------------------------------------------------------------------

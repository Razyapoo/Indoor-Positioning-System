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

    XGBoosterFree(booster);
    videoProcessorThread->quit();
}

int VideoProcessor::loadModelParams(const QString& filename) {
    // QFile file(filename);
    // if (!file.open(QIODevice::ReadOnly)) {
    //     qWarning("Couldn't open the file.");
    //     return;
    // }

    // QByteArray data = file.readAll();
    // QJsonDocument doc(QJsonDocument::fromJson(data));

    // polynRegressionParams = doc.object();


    // XGBooster Regressor
    // XGBoosterCreate(NULL, 0, &booster);
    // XGBoosterLoadModel(booster, filename.toStdString().c_str());
    int result = XGBoosterCreate(NULL, 0, &booster);
    if (result == 0) {
        result = XGBoosterLoadModel(booster, filename.toStdString().c_str());
        if (result == -1) {
            booster = nullptr;
        }
    }

    return result;
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
                    std::vector<DetectionResult> detectionsVector;
                    detectPeople(frame, detectionsVector);
                    if (i != (frameRangeToExport.size() - 1)) {
                        emit requestFindUWBMeasurementAndExport(frameRangeToExport[i], i, exportType, detectionsVector, false);
                    } else {
                        emit requestFindUWBMeasurementAndExport(frameRangeToExport[i], i, exportType, detectionsVector, true);
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


            if (isPredictionRequested)
            {
                std::vector<DetectionResult> detectionsVector;
                detectPeople(frame, detectionsVector);
                std::pair<double, double> coordinates;
                if (detectionsVector.size()) {
                    for (const DetectionResult& detection: detectionsVector){
                        predictWorldCoordinates(detection);
                        // std::string coordinatesText = "(" + std::to_string(coordinates.first) + ", " + std::to_string(coordinates.second) + ")" ;
                        // cv::putText(frame, coordinatesText, cv::Point(detection.bottomEdgeCenter.x(), detection.bottomEdgeCenter.y()), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
                        // qDebug() << coordinatesText;
                    }
                }
            }


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

std::pair<double, double> VideoProcessor::predictWorldCoordinates(const DetectionResult& detection) {
    // // Construct polynomial features from pixel coordinates
    // Eigen::MatrixXd features(1, 6); // Adjust size based on the degree and interaction terms
    // features << 1, x_pixel, y_pixel, std::pow(x_pixel, 2), std::pow(y_pixel, 2), x_pixel * y_pixel;

    // QJsonArray coeffs_X_array = polynRegressionParams["model_X_coefficients"].toArray();
    // Eigen::VectorXd coeffs_X(coeffs_X_array.size());
    // for (int i = 0; i < coeffs_X_array.size(); ++i) {
    //     coeffs_X[i] = coeffs_X_array[i].toDouble();
    // }
    // double intercept_X = polynRegressionParams["model_X_intercept"].toDouble();

    // QJsonArray coeffs_Y_array = polynRegressionParams["model_Y_coefficients"].toArray();
    // Eigen::VectorXd coeffs_Y(coeffs_Y_array.size());
    // for (int i = 0; i < coeffs_Y_array.size(); ++i) {
    //     coeffs_Y[i] = coeffs_Y_array[i].toDouble();
    // }
    // double intercept_Y = polynRegressionParams["model_Y_intercept"].toDouble();

    // // Predict world coordinates
    // double X_world = (features * coeffs_X).value() + intercept_X;
    // double Y_world = (features * coeffs_Y).value() + intercept_Y;

    // std::pair<double, double> coordinates = std::make_pair(X_world, Y_world);
    // return coordinates;

    std::pair<double, double> coordinates;

    if (predictionType == PredictionType::PredictionByModel)
    {
        // XGBooster Regressor
        DMatrixHandle dmatrix;
        const float pixelCoordinates[] = {static_cast<float>(detection.bottomEdgeCenter.x()), static_cast<float>(detection.bottomEdgeCenter.y())};
        XGDMatrixCreateFromMat(pixelCoordinates, 1, 2, NAN, &dmatrix);

        // Make prediction
        bst_ulong outLen;
        const float* outResult;
        XGBoosterPredict(booster, dmatrix, 0, 0, 0, &outLen, &outResult);

        // Output predicted values
        // std::cout << "Predicted Real-World Coordinates: (" << out_result[0] << ", " << out_result[1] << ")" << std::endl;

        // coordinates = std::make_pair(static_cast<double>(out_result[0]), static_cast<double>(out_result[1]));
        qDebug() << "Real-World coordinates predicted by model: (" << outResult[0] << ", " << outResult[1] << ")";


        // Cleanup
        XGDMatrixFree(dmatrix);
    }

    if (predictionType == PredictionType::PredictionByHeight) {
        // int centerX = detection.bbox.x;
        // int centerY = detection.bbox.y;
        // int width = detection.bbox.width;
        int height = detection.bbox.height;

        int imageX = detection.bottomEdgeCenter.x();
        int imageY = detection.bottomEdgeCenter.y();

        // Camera parameters are computed for 640x360 size of an image. Need a scale factor for 640x640.
        double scaleX = detectionFrameSize.width / cameraFrameSize.width;
        double scaleY = (double)detectionFrameSize.height / (double)cameraFrameSize.height;


        double fxAdjusted = optimalCameraMatrix[0] * scaleX;
        double fyAdjusted = optimalCameraMatrix[4] * scaleY;
        double cxAdjusted = optimalCameraMatrix[2] * scaleX;
        double cyAdjusted = optimalCameraMatrix[5] * scaleY;

        double distance = (1760 * fyAdjusted) / height;

        // cx and fx from intrinsic calibration
        double worldX = (imageX - cxAdjusted) * (distance / fxAdjusted);
        // cy and fy from intrinsic calibration
        double worldY = (imageY - cyAdjusted) * (distance / fyAdjusted);

        qDebug() << "Pixel Coordinates: (" << imageX << ", "<< imageY << ")";
        qDebug() << "   Real-World coordinates calculated by height: (" << worldX << ", " << distance << ")" << " Z: " << worldY;

    }

    return coordinates;

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
int VideoProcessor::setPredict(bool toPredict, PredictionType type) {

    predictionType = type;
    if (type == PredictionType::PredictionByModel) {
        if (!booster) {
            return -1;
        }
        isPredictionByModelRequested = toPredict;
    } else if (type == PredictionType::PredictionByHeight) {
        if (!optimalCameraMatrix.size()) {
            return -1;
        }
        isPredictionByHeightRequested = toPredict;
    }
    isPredictionRequested = toPredict;

    return 0;
}


void VideoProcessor::setOptimalCameraMatrix(std::vector<double> &&matrix) {
    optimalCameraMatrix = std::move(matrix);
}

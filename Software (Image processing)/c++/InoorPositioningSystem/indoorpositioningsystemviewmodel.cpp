#include "indoorpositioningsystemviewmodel.h"


IndoorPositioningSystemViewModel::IndoorPositioningSystemViewModel(QObject *parent)
    : QObject(parent)
    , frameQueue(100)

{

    dataProcessor = std::make_unique<DataProcessor>(frameQueue);
    videoProcessor = std::make_unique<VideoProcessor>(frameQueue, dataProcessor.get());

    frameTimer = new QTimer(this);
    frameTimer->setInterval(60); // 3500 for human detector

    dataProcessor = std::make_unique<DataProcessor>(frameQueue);
    videoProcessor = std::make_unique<VideoProcessor>(frameQueue, dataProcessor.get());
    isVideoOpened = false;
    toPredictByModel = false;
    toPredictByHeight = false;


    connect(frameTimer, &QTimer::timeout, this, &IndoorPositioningSystemViewModel::checkForDisplay);
    connect(this, &IndoorPositioningSystemViewModel::frameIsReady, this, &IndoorPositioningSystemViewModel::updateDataDisplay);
    connect(this, &IndoorPositioningSystemViewModel::requestProcessVideo, videoProcessor.get(), &VideoProcessor::processVideo);


    connect(videoProcessor.get(), &VideoProcessor::seekingDone, this, &IndoorPositioningSystemViewModel::afterSeeking);
    connect(dataProcessor.get(), &DataProcessor::exportProgressUpdated, this, &IndoorPositioningSystemViewModel::onExportProgressUpdated);
    connect(videoProcessor.get(), &VideoProcessor::exportFinished, this, &IndoorPositioningSystemViewModel::onExportFinished, Qt::BlockingQueuedConnection);


}

IndoorPositioningSystemViewModel::~IndoorPositioningSystemViewModel()
{
    videoProcessor->stopProcessing();
}

void IndoorPositioningSystemViewModel::openVideo(const QString& directory)
{
    // frameTimer->stop();

    std::string videoFileName, videoTimestampsFileName, UWBDataFileName;
    bool missingFile = false;
    QStringList missingFiles;

    if (!directory.isEmpty()) {
        QDir qDirectory(directory);

        videoFileName = qDirectory.filePath("video.avi").toStdString();
        UWBDataFileName = qDirectory.filePath("UWB_timestamps.txt").toStdString();
        videoTimestampsFileName = qDirectory.filePath("video_timestamps.txt").toStdString();

        if (!QFile::exists(QString::fromStdString(videoFileName))) {
            missingFiles << "video.avi";
            missingFile = true;
        }
        if (!QFile::exists(QString::fromStdString(UWBDataFileName))) {
            missingFiles << "UWB_timestamps.txt";
            missingFile = true;
        }
        if (!QFile::exists(QString::fromStdString(videoTimestampsFileName))) {
            missingFiles << "video_timestamps.txt";
            missingFile = true;
        }
    }

    if (missingFile) {
        QString message = "Missing required files: " + missingFiles.join(", ");
        emit videoOpenFailed(false, message);
    } else {
        dataProcessor->loadData(UWBDataFileName, videoTimestampsFileName);
        videoProcessor->init(videoFileName);
        int totalFrames = dataProcessor->getTotalFrames();
        isVideoOpened = true;
        isPlaying = true;
        isExportState = false;
        emit requestProcessVideo();
        toPredictByModel = false;

        long long videoDuration = dataProcessor->getVideoTimestampById(totalFrames - 1) - dataProcessor->getVideoTimestampById(0);
        emit videoOpened(totalFrames, videoDuration);
    }

    if (isVideoOpened) {
        frameTimer->start();
    }
}

void IndoorPositioningSystemViewModel::loadModelParams(const QString& selectedFile){
    // frameTimer->stop();

    if (!selectedFile.isEmpty()) {
        int result = videoProcessor->loadModelParams(selectedFile);
        if (result == -1) {
            emit modelParamsLoaded(false, "Failed to load model!");
            toPredictByModel = false;
        } else {
            result = videoProcessor->setPredict(toPredictByModel, PredictionType::PredictionByModel);
            if (result != -1) {
                frameQueue.clear();
                emit modelParamsLoaded(true, "Model was loaded successfully!");
            } else {
                emit modelParamsLoaded(false, "Failed to load model!");
                toPredictByModel = false;
            }
        }
    } else {
        emit modelParamsLoaded(false, "No file was selected!");
        toPredictByModel = false;
    }

    if (isVideoOpened) {
        frameTimer->start();
    }

}

void IndoorPositioningSystemViewModel::loadIntrinsicCalibrationParams(const QString& selectedFile){
    if (selectedFile.isEmpty()) {
        emit intrinsicCalibrationParamsLoaded(false, "No file was selected!");
        return;
    }

    QFile file(selectedFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString warningMessage = QString("Error opening file: %1").arg(file.errorString());
        emit intrinsicCalibrationParamsLoaded(false, warningMessage);
        toPredictByHeight = false;
        return;
    }

    bool foundOptimalMatrixParameters = false;
    bool foundCameraMatrixParameters = false;
    bool foundDistCoeffsParameters = false;

    std::vector<double> optimalCameraMatrix;
    std::vector<double> cameraMatrix;
    std::vector<double> distCoeffs;

    QXmlStreamReader xmlReader(&file);
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if (token == QXmlStreamReader::StartElement) {
            auto const bytes = xmlReader.name().toUtf8();
            std::string tag = std::string(bytes.constData(), bytes.length());
            if (tag == "optimalCameraMatrix") {
                foundOptimalMatrixParameters = true;
            } else if (tag == "cameraMatrix") {
                foundCameraMatrixParameters = true;
            } else if (tag == "distortionCoeffs") {
                foundDistCoeffsParameters = true;
            } else if (tag == "data") {
                QString data = xmlReader.readElementText();
                static QRegularExpression pattern("\\s+");
                QStringList values = data.split(pattern, Qt::SkipEmptyParts);
                if (foundOptimalMatrixParameters) {
                    for (const QString& value : values) {
                        optimalCameraMatrix.push_back(value.toDouble());
                    }
                    foundOptimalMatrixParameters = false;
                } else if (foundCameraMatrixParameters) {
                    for (const QString& value : values) {
                        cameraMatrix.push_back(value.toDouble());
                    }
                    foundCameraMatrixParameters = false;
                } else if (foundDistCoeffsParameters) {
                    for (const QString& value : values) {
                        distCoeffs.push_back(value.toDouble());
                    }
                    foundDistCoeffsParameters = false;
                }
            }
        }
    }

    if (xmlReader.hasError()) {
        QString warningMessage = QString("Error parsing XML: %1").arg(xmlReader.errorString());
        emit intrinsicCalibrationParamsLoaded(false, warningMessage);
        toPredictByHeight = false;
    } else {
        if (optimalCameraMatrix.size() == 9) {
            videoProcessor->setOptimalCameraMatrix(std::move(optimalCameraMatrix));
        } else {
            emit intrinsicCalibrationParamsLoaded(false, "Optional. Optimal camera matrix parameters were not found or are incomplete.");
        }

        if (distCoeffs.size() == 5) { // Assuming 5 distortion coefficients
            videoProcessor->setDistCoeffs(std::move(distCoeffs));
        } else {
            emit intrinsicCalibrationParamsLoaded(false, "Optional. Distortion coefficients were not found or are incomplete.");
        }

        if (cameraMatrix.size() == 9) {
            videoProcessor->setCameraMatrix(std::move(cameraMatrix));
            int result = videoProcessor->setPredict(toPredictByHeight, PredictionType::PredictionByHeight);
            if (result != -1) {
                frameQueue.clear();
                emit modelParamsLoaded(true, "Model was loaded successfully!");
            } else {
                emit modelParamsLoaded(true, "Failed to load model!");
            }
        } else {
            toPredictByHeight = false;
            emit intrinsicCalibrationParamsLoaded(false, "Mandatory. Camera matrix parameters were not found or are incomplete. Method cannot be started.");
        }
    }

    file.close();

    if (isVideoOpened) {
        frameTimer->start();
    }
}


void IndoorPositioningSystemViewModel::checkForDisplay() {
    UWBVideoData data;

    if (isPlaying && !isExportState) {
        if (frameQueue.dequeue(data)) {
            emit frameIsReady(data);
        }
    }
}

void IndoorPositioningSystemViewModel::updateDataDisplay(const UWBVideoData& data) {

    if (data.videoData.qImage.isNull() && data.videoData.id == -1) {
        frameTimer->stop();
        emit finishedVideoProcessing();
    } else {
        emit dataUpdated(data.videoData.qImage, data.videoData.id, QString::number(data.videoData.timestamp));


        for (const UWBData& tag: data.uwbData)
        {
            QString tagTimestampText = QString::number(tag.timestamp);
            QString anchor101DistanceText = QString::number(tag.anchorList[0].distance, 'f', 6);
            QString anchor102DistanceText = QString::number(tag.anchorList[1].distance, 'f', 6);

            emit uwbDataUpdated(tag.tagID, tagTimestampText, anchor101DistanceText, anchor102DistanceText);
            emit updateTagPosition(tag.coordinates, tag.tagID);

        }


        long long currentTimeInMSeconds = data.videoData.timestamp - dataProcessor->getVideoTimestampById(0);
        emit durationUpdated(data.videoData.id, currentTimeInMSeconds);
        lastPosition = data.videoData.id;
    }

}

bool IndoorPositioningSystemViewModel::isPlayingCheck() const {
    return isPlaying;
}

void IndoorPositioningSystemViewModel::play()
{
    if (isVideoOpened) {
        if (!isPlaying) {
            isPlaying = true;
            frameTimer->start();
        }
    } else {
        emit showWarning("Missing video", "No video is open. Please open a one!");
    }
}

void IndoorPositioningSystemViewModel::pause()
{
    if (isVideoOpened) {
        if (isPlaying) {
            isPlaying = false;
            frameTimer->stop();
        }
    } else {
        emit showWarning("Missing video", "No video is open. Please open a one!");
    }
}

void IndoorPositioningSystemViewModel::startTimer() {
    frameTimer->start();
}

void IndoorPositioningSystemViewModel::stopTimer() {
    frameTimer->stop();
}

void IndoorPositioningSystemViewModel::updateVideoPosition(int position){
    if (frameTimer->isActive()) {
        frameTimer->stop();
    }

    long long setTimeInMSeconds = dataProcessor->getVideoTimestampById(position - 1) - dataProcessor->getVideoTimestampById(0);
    QTime setTime = QTime(0, 0).addMSecs(static_cast<int>(setTimeInMSeconds));
    QString currentTime = setTime.toString("HH:mm:ss");
    seekPosition = position;

    emit positionUpdated(currentTime);
}


void IndoorPositioningSystemViewModel::afterSeeking() {
    if (isPlaying) {
        frameTimer->start();
    }
}

void IndoorPositioningSystemViewModel::seekToFrame(){
    videoProcessor->pauseProcessing();
    frameQueue.clear();
    videoProcessor->seekToFrame(seekPosition);
    videoProcessor->resumeProcessing();
}

void IndoorPositioningSystemViewModel::setFrameByFrameExportRange(const QTime& startTime, const QTime& endTime) {
    long long startTimeSec = (startTime.hour() * 3600 + startTime.minute() * 60 + startTime.second());
    long long endTimeSec = (endTime.hour() * 3600 + endTime.minute() * 60 + endTime.second());

    long long startTimestampMS = startTimeSec * 1000 + dataProcessor->getVideoTimestampById(0);
    long long endTimestampMS = endTimeSec * 1000 + dataProcessor->getVideoTimestampById(0);

    // OpenCV read frames by ID. These IDs correspond to IDs stored in the video_timestamps.txt, which identify frames recorded by OpenCV.
    // These IDs are read into VideoData data together with timestamps form video_timestamps.txt.
    int startFrameIndex = dataProcessor->binarySearchVideoFrameID(startTimestampMS);
    int endFrameIndex = dataProcessor->binarySearchVideoFrameID(endTimestampMS);

    int totalExportDuration = endFrameIndex - startFrameIndex;

    std::vector<int> frameRangeToExport;
    if (totalExportDuration > 0) {
        for (int i = startFrameIndex; i <= endFrameIndex; ++i) {
            frameRangeToExport.push_back(i);
        }
        setupExportConfiguration(frameRangeToExport, ExportType::FrameByFrameExport);

    } else {
        emit showExportWarning("Warning! Wrong time range.", "Time range is set wrong. End time should be larger than start time.", ExportType::FrameByFrameExport);
    }
}

void IndoorPositioningSystemViewModel::setupExportConfiguration(const std::vector<int>& frameRangeToExport, ExportType type) {
    isPlaying = false;
    if (frameTimer->isActive()) {
        frameTimer->stop();
    }
    isExportState = true;

    videoProcessor->pauseProcessing();
    frameQueue.interruptionRequest();
    videoProcessor->setFrameRangeToExport(frameRangeToExport, type);
    videoProcessor->resumeProcessing();

    totalExportDuration = frameRangeToExport.size();
    emit showExportProcessDialog();
}

void IndoorPositioningSystemViewModel::onExportProgressUpdated(int index) {
    int proportion = (index * 100) / totalExportDuration;
    emit updateExportProgress(proportion);
}

// Export only a specific frame from each segment. Segment corresponds to a stationary period in video recording. This is usefull for faster data analysis.
// void IndoorPositioningSystemViewModel::onSegmentFramesExport() {
//     std::vector<int> frameRangeToExport = dataProcessor->getSegmentFrameIDs();
//     if (frameRangeToExport.size()) {
//         setupExportConfiguration(frameRangeToExport, ExportType::SegmentFramesExport);
//     } else {
//         emit showExportWarning("Warning! Wrong time range.", "Time range is set wrong. Please set segment frame IDs to export.", ExportType::SegmentFramesExport);
//     }
// }

void IndoorPositioningSystemViewModel::stopExport() {
    videoProcessor->stopExport();
}

void IndoorPositioningSystemViewModel::onExportFinished(bool success) {
    isPlaying = true;
    isExportState = false;

    videoProcessor->pauseProcessing();
    frameQueue.clear();
    videoProcessor->seekToFrame(lastPosition);
    videoProcessor->resumeProcessing();

    emit exportFinished(success);
}

void IndoorPositioningSystemViewModel::setPredictByModel(bool toPredict) {
    toPredictByModel = toPredict;
}

void IndoorPositioningSystemViewModel::setPredictByHeight(bool toPredict) {
    toPredictByHeight = toPredict;
}

void IndoorPositioningSystemViewModel::predict(PredictionType type) {
    int result = -1;
    videoProcessor->pauseProcessing();

    if (type == PredictionType::PredictionByModel) {
        toPredictByModel = !toPredictByModel;
        result = videoProcessor->setPredict(toPredictByModel, type);
    } else if (type == PredictionType::PredictionByHeight) {
        toPredictByHeight = !toPredictByHeight;
        result = videoProcessor->setPredict(toPredictByHeight, type);
    }

    if (result != -1) {
        frameQueue.clear();
    } else {
        frameTimer->stop();

        emit modelNotLoaded(type);

        frameTimer->start();
    }
    videoProcessor->resumeProcessing();
}

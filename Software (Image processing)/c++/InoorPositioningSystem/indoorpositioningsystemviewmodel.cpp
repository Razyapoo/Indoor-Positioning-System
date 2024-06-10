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
    toPredictByPixelToReal = false;
    toPredictionByOptical = false;


    connect(frameTimer, &QTimer::timeout, this, &IndoorPositioningSystemViewModel::checkForDisplay);
    connect(this, &IndoorPositioningSystemViewModel::frameIsReady, this, &IndoorPositioningSystemViewModel::updateDataDisplay);
    connect(this, &IndoorPositioningSystemViewModel::requestProcessVideo, videoProcessor.get(), &VideoProcessor::processVideo);


    connect(videoProcessor.get(), &VideoProcessor::seekingDone, this, &IndoorPositioningSystemViewModel::afterSeeking);
    connect(dataProcessor.get(), &DataProcessor::exportProgressUpdated, this, &IndoorPositioningSystemViewModel::onExportProgressUpdated);
    connect(videoProcessor.get(), &VideoProcessor::exportFinished, this, &IndoorPositioningSystemViewModel::onExportFinished, Qt::BlockingQueuedConnection);
    connect(videoProcessor.get(), &VideoProcessor::requestChangePredictionButtonName, this, &IndoorPositioningSystemViewModel::onChangePredictionButtonName);
    connect(videoProcessor.get(), &VideoProcessor::humanDetectorNotInitialized, this, &IndoorPositioningSystemViewModel::onHumanDetectorNotInitialized);


    connect(dataProcessor.get(), &DataProcessor::requestShowAvailableTags, this, &IndoorPositioningSystemViewModel::showAvailableTags);
    connect(dataProcessor.get(), &DataProcessor::requestShowAvailableAnchors, this, &IndoorPositioningSystemViewModel::showAvailableAnchors);
    connect(dataProcessor.get(), &DataProcessor::requestShowPlotDistancesVsTimestamps, this, &IndoorPositioningSystemViewModel::showPlotDistancesVsTimestamps);
    connect(dataProcessor.get(), &DataProcessor::requestShowPlotRollingDeviations, this, &IndoorPositioningSystemViewModel::showPlotRollingDeviations);
    connect(dataProcessor.get(), &DataProcessor::requestShowDatasetSegments, this, &IndoorPositioningSystemViewModel::showDatasetSegments);
    connect(dataProcessor.get(), &DataProcessor::requestShowOriginalVsAdjustedDistances, this, &IndoorPositioningSystemViewModel::showOriginalVsAdjustedDistances);

    connect(this, &IndoorPositioningSystemViewModel::requestSetRangeForDataAnalysis, dataProcessor.get(), &DataProcessor::setRangeForDataAnalysis);
    connect(this, &IndoorPositioningSystemViewModel::requestCollectDataForPlotDistancesVsTimestamps, dataProcessor.get(), &DataProcessor::collectDataForPlotDistancesVsTimestamps);
    connect(this, &IndoorPositioningSystemViewModel::requestCalculateRollingDeviation, dataProcessor.get(), &DataProcessor::calculateRollingDeviation);
    connect(this, &IndoorPositioningSystemViewModel::requestSplitDataset, dataProcessor.get(), &DataProcessor::splitDataset);
    connect(this, &IndoorPositioningSystemViewModel::requestCalculatePolynomialRegression, dataProcessor.get(), &DataProcessor::calculatePolynomialRegression);
    connect(this, &IndoorPositioningSystemViewModel::requestUpdateOriginalWithAdjustedValues, dataProcessor.get(), &DataProcessor::updateOriginalWithAdjustedValues);
    connect(this, &IndoorPositioningSystemViewModel::requestCollectDataForTag, dataProcessor.get(), &DataProcessor::collectDataForTag);


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
       emit showWarning("Failed to load", message);
    } else {
        dataProcessor->loadData(UWBDataFileName, videoTimestampsFileName);
        videoProcessor->init(videoFileName);
        int totalFrames = dataProcessor->getTotalFrames();
        isVideoOpened = true;
        _isPlaying = true;
        isExportState = false;
        emit requestProcessVideo();
        toPredictByPixelToReal = false;

        long long videoDuration = dataProcessor->getVideoTimestampById(totalFrames - 1) - dataProcessor->getVideoTimestampById(0);
        emit videoOpened(totalFrames, videoDuration);
    }

    if (isVideoOpened && _isPlaying) {
        frameTimer->start();
    }
}

void IndoorPositioningSystemViewModel::loadHumanDetectorWeights(const QString& directory) {
    bool missingFile = false;
    std::string configFileName, weightsFileName;
    QStringList missingFiles;

    if (!directory.isEmpty()) {
        QDir qDirectory(directory);

        QStringList configFilter;
        configFilter << "*.cfg";
        QStringList configFiles = qDirectory.entryList(configFilter, QDir::Files);

        if (configFiles.isEmpty()) {
            missingFiles << "configuration file (*.cfg)";
            missingFile = true;
        } else if (configFiles.size() > 1){
            emit showWarning("Failed to load", "More than one configuration file found.");
            return;
        } else {
            configFileName = qDirectory.filePath(configFiles.first()).toStdString();
        }

        QStringList weightsFilter;
        weightsFilter << "*.weights";
        QStringList weightsFiles = qDirectory.entryList(weightsFilter, QDir::Files);

        if (weightsFiles.isEmpty()) {
            missingFiles << "weights file (*.weights)";
            missingFile = true;
        } else if (weightsFiles.size() > 1){
            emit showWarning("Failed to load", "More than one weights file found.");
            return;
        } else {
            weightsFileName = qDirectory.filePath(weightsFiles.first()).toStdString();
        }
    }

    if (missingFile) {
        QString message = "Missing required files: " + missingFiles.join(", ");
        emit showWarning("Failed to load", message);
    } else {
        videoProcessor->initHumanDetector(configFileName, weightsFileName);
        emit weightsLoaded(true, "Weights are successfully loaded!");
    }

}

void IndoorPositioningSystemViewModel::loadPixelToRealModelParams(const QString& selectedFile){
    // frameTimer->stop();

    if (!selectedFile.isEmpty()) {
        int result = videoProcessor->loadPixelToRealModelParams(selectedFile);
        if (result == -1) {
            emit modelParamsLoaded(false, "Failed to load model!");
            toPredictByPixelToReal = false;
        } else {
            result = videoProcessor->setPredict(toPredictByPixelToReal, PredictionType::PredictionByPixelToReal);
            if (result != -1) {
                frameQueue.clear();
                emit modelParamsLoaded(true, "Model was loaded successfully!");
            } else {
                emit modelParamsLoaded(false, "Failed to load model!");
                toPredictByPixelToReal = false;
            }
        }
    } else {
        emit modelParamsLoaded(false, "No file was selected!");
        toPredictByPixelToReal = false;
    }

    if (isVideoOpened && _isPlaying) {
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
        toPredictionByOptical = false;
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
        toPredictionByOptical = false;
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
            int result = videoProcessor->setPredict(toPredictionByOptical, PredictionType::PredictionByOptical);
            if (result != -1) {
                frameQueue.clear();
                emit modelParamsLoaded(true, "Model was loaded successfully!");
            } else {
                emit modelParamsLoaded(true, "Failed to load model!");
            }
        } else {
            toPredictionByOptical = false;
            emit intrinsicCalibrationParamsLoaded(false, "Mandatory. Camera matrix parameters were not found or are incomplete. Method cannot be started.");
        }
    }

    file.close();

    if (isVideoOpened && _isPlaying) {
        frameTimer->start();
    }
}


void IndoorPositioningSystemViewModel::checkForDisplay() {
    UWBVideoData data;

    if (_isPlaying && !isExportState) {
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
            emit uwbDataUpdated(tag);
            // std::pair<double, double> coordinates = std::make_pair<double, double>(static_cast<double>(tag.coordinates.x()), static_cast<double>(tag.coordinates.y()));
            emit updateTagPosition(tag.coordinates, tag.tagID);
        }

        if (data.pixelToRealCoordinates.size()) {
            int i = 1;
            for (const QPointF& coordinates: data.pixelToRealCoordinates) {
                emit updatePixelToRealPosition(coordinates, i);
                i++;
            }
        }

        if (data.opticalCoordinates.size()) {
            int i = 1;
            for (const QPointF& coordinates: data.opticalCoordinates) {
                emit updateOpticalPosition(coordinates, i);
                i++;
            }
        }


        long long currentTimeInMSeconds = data.videoData.timestamp - dataProcessor->getVideoTimestampById(0);
        emit durationUpdated(data.videoData.id, currentTimeInMSeconds);
        lastPosition = data.videoData.id;
    }

}

bool IndoorPositioningSystemViewModel::isPlaying() const {
    return _isPlaying;
}

void IndoorPositioningSystemViewModel::play()
{
    if (isVideoOpened) {
        if (!_isPlaying) {
            _isPlaying = true;
            frameTimer->start();
        }
    } else {
        emit showWarning("Missing video", "No video is open. Please open a one!");
    }
}

void IndoorPositioningSystemViewModel::pause()
{
    if (isVideoOpened) {
        if (_isPlaying) {
            _isPlaying = false;
            frameTimer->stop();
        }
    } else {
        emit showWarning("Missing video", "No video is open. Please open a one!");
    }
}

void IndoorPositioningSystemViewModel::startTimer() {
    if (isVideoOpened && _isPlaying) {
        frameTimer->start();
    }
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
    if (_isPlaying) {
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
    _isPlaying = false;
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
void IndoorPositioningSystemViewModel::onSegmentFramesExport() {
    std::vector<int> frameRangeToExport = dataProcessor->getSegmentFrameIDs();
    if (frameRangeToExport.size()) {
        setupExportConfiguration(frameRangeToExport, ExportType::SegmentFramesExport);
    } else {
        emit showExportWarning("Warning! Wrong time range.", "Time range is set wrong. Please set segment frame IDs to export.", ExportType::SegmentFramesExport);
    }
}

void IndoorPositioningSystemViewModel::stopExport() {
    videoProcessor->stopExport();
}

void IndoorPositioningSystemViewModel::onExportFinished(bool success) {
    _isPlaying = true;
    isExportState = false;

    videoProcessor->pauseProcessing();
    frameQueue.clear();
    videoProcessor->seekToFrame(lastPosition);
    videoProcessor->resumeProcessing();

    emit exportFinished(success);
}

void IndoorPositioningSystemViewModel::setPredictByPixelToReal(bool toPredict) {
    toPredictByPixelToReal = toPredict;
}

void IndoorPositioningSystemViewModel::setPredictionByOptical(bool toPredict) {
    toPredictionByOptical = toPredict;
}

void IndoorPositioningSystemViewModel::predict(PredictionType type) {
    int result = -1;
    videoProcessor->pauseProcessing();

    if (type == PredictionType::PredictionByPixelToReal) {
        toPredictByPixelToReal = !toPredictByPixelToReal;
        result = videoProcessor->setPredict(toPredictByPixelToReal, type);
    } else if (type == PredictionType::PredictionByOptical) {
        toPredictionByOptical = !toPredictionByOptical;
        result = videoProcessor->setPredict(toPredictionByOptical, type);
    }

    if (!result) {
        frameQueue.clear();
    } else if (result == -1) {
        frameTimer->stop();
        emit modelNotLoaded(type);
        frameTimer->start();
    } else if (result == -2) {
        toPredictionByOptical = false;
        toPredictByPixelToReal = false;
    }
    videoProcessor->resumeProcessing();
}

// Analysis Window

void IndoorPositioningSystemViewModel::setRangeForDataAnalysis(const long long startTimeSec, const long long endTimeSec) {
    emit requestSetRangeForDataAnalysis(startTimeSec, endTimeSec);
}

void IndoorPositioningSystemViewModel::collectDataForPlotDistancesVsTimestamps(const int anchorID) {
    emit requestCollectDataForPlotDistancesVsTimestamps(anchorID);
}

void IndoorPositioningSystemViewModel::calculateRollingDeviation(const int windowSize) {
    emit requestCalculateRollingDeviation(windowSize);
}

void IndoorPositioningSystemViewModel::splitDataset(const double threshold) {
    emit requestSplitDataset(threshold);
}

void IndoorPositioningSystemViewModel::calculatePolynomialRegression(const std::vector<double> &referenceValues) {
    emit requestCalculatePolynomialRegression(referenceValues);
}

void IndoorPositioningSystemViewModel::updateOriginalWithAdjustedValues() {
    emit requestUpdateOriginalWithAdjustedValues();
}

void IndoorPositioningSystemViewModel::collectDataForTag(const QString &tagIDText) {
    emit requestCollectDataForTag(tagIDText);
}

void IndoorPositioningSystemViewModel::showAvailableTags(const std::vector<int> &availableTagIDs) {
    emit requestShowAvailableTags(availableTagIDs);
}

void IndoorPositioningSystemViewModel::showAvailableAnchors(const std::vector<int> &availableAnchorIDs) {
   emit requestShowAvailableAnchors(availableAnchorIDs);
}

void IndoorPositioningSystemViewModel::showPlotDistancesVsTimestamps(const std::vector<long long> &timestamps, std::vector<double *> distances) {
    emit requestShowPlotDistancesVsTimestamps(timestamps, distances);
}

void IndoorPositioningSystemViewModel::showPlotRollingDeviations(const std::vector<long long> &timestamps, const std::vector<double> &deviations) {
    emit requestShowPlotRollingDeviations(timestamps, deviations);
}

void IndoorPositioningSystemViewModel::showDatasetSegments(const std::vector<double> &datasetSegmentMeans) {
    emit requestShowDatasetSegments(datasetSegmentMeans);
}

void IndoorPositioningSystemViewModel::showOriginalVsAdjustedDistances(const std::vector<long long> &timestampsToAnalyze, std::vector<double *> distancesToAnalyzeOriginal, const std::vector<double> &distancesToAnalyzeAdjusted) {
    emit requestShowOriginalVsAdjustedDistances(timestampsToAnalyze, distancesToAnalyzeOriginal, distancesToAnalyzeAdjusted);
}

void IndoorPositioningSystemViewModel::onChangePredictionButtonName(PredictionType type, bool isPredictionRequested) {
    emit requestChangePredictionButtonName(type, isPredictionRequested);
}

void IndoorPositioningSystemViewModel::onHumanDetectorNotInitialized() {
    emit humanDetectorNotInitialized();
}

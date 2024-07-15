#include "indoorpositioningsystemviewmodel.h"


IndoorPositioningSystemViewModel::IndoorPositioningSystemViewModel(QObject *parent)
    : QObject(parent)
    , frameQueue(100) // !!Very important optimization. Should be set reasonable value. Otherwise causes high load on machine!!

{
    dataProcessor = std::make_unique<DataProcessor>(frameQueue); // Thread is created inside a class for its easier handling
    videoProcessor = std::make_unique<VideoProcessor>(frameQueue, dataProcessor.get()); // Thread is created inside a class for its easier handling

    // Timer is used to handle fps of video player
    frameTimer = new QTimer(this);
    frameTimer->setInterval(60); // corresponds to 17 fps at which videos are recorded

    isVideoOpened = false;
    toPredictByPixelToReal = false;
    toPredictionByOptical = false;

    connect(frameTimer, &QTimer::timeout, this, &IndoorPositioningSystemViewModel::checkForDisplay);
    connect(this, &IndoorPositioningSystemViewModel::frameIsReady, this, &IndoorPositioningSystemViewModel::updateDataDisplay);
    connect(this, &IndoorPositioningSystemViewModel::requestProcessVideo, videoProcessor.get(), &VideoProcessor::processVideo);


    connect(videoProcessor.get(), &VideoProcessor::seekingDone, this, &IndoorPositioningSystemViewModel::afterSeeking);
    connect(videoProcessor.get(), &VideoProcessor::exportFinished, this, &IndoorPositioningSystemViewModel::onExportFinished, Qt::BlockingQueuedConnection);
    connect(videoProcessor.get(), &VideoProcessor::humanDetectorNotInitialized, this, &IndoorPositioningSystemViewModel::onHumanDetectorNotInitialized);
    connect(videoProcessor.get(), &VideoProcessor::distCoeffLoaded, this, &IndoorPositioningSystemViewModel::onDistCoeffsLoaded);

    connect(dataProcessor.get(), &DataProcessor::exportProgressUpdated, this, &IndoorPositioningSystemViewModel::onExportProgressUpdated);
    connect(dataProcessor.get(), &DataProcessor::requestChangePredictionButtonName, this, &IndoorPositioningSystemViewModel::onChangePredictionButtonName);

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


// ------------------------- Load files --------------------------------------------------------------------
// Open Video + UWB data
// Mandatory files: video.avi, UWB_timestamps.txt, video_timestamps.txt
bool IndoorPositioningSystemViewModel::openVideo(const QString& directory)
{
    anchorPositions.clear();
    std::string videoFileName, videoTimestampsFileName, UWBDataFileName;
    std::string directoryPath = directory.toStdString();
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
       return false;
    } else {
        dataProcessor->loadData(directoryPath, UWBDataFileName, videoTimestampsFileName); // Load data
        videoProcessor->init(videoFileName); // Load video
        frameQueue.clear(); // Empty Frame queue in case new video is opened
    }

    return true;
}

// If it is called by bytton "Predict by Pixel-to-Real" than the prediction starts immediately
// If it is called from 'File' option in the toolbar than the prediction do not start immediately
void IndoorPositioningSystemViewModel::loadPixelToRealModelParams(const QString& selectedFile){
    int result = -1;
    if (!selectedFile.isEmpty()) {
        result = dataProcessor->loadPixelToRealModelParams(selectedFile);
        if (result == -1) {
            emit modelParamsLoaded(false, "Failed to load model!");
            toPredictByPixelToReal = false;
        } else if (toPredictByPixelToReal) {
            result = videoProcessor->setPredict(toPredictByPixelToReal);
            if (result != -1) {
                result = dataProcessor->setPredict(toPredictByPixelToReal, PredictionType::PredictionByPixelToReal);
                if (result != -1) {
                    frameQueue.clear();
                    emit modelParamsLoaded(true, "Model was loaded successfully!");
                } else {
                    emit modelParamsLoaded(false, "Failed to load model!");
                    toPredictByPixelToReal = false;
                }
            } else {
                emit humanDetectorNotInitialized();
            }
        }
    } else {
        emit modelParamsLoaded(false, "No file was selected!");
        toPredictByPixelToReal = false;
    }

    // if some video is opened and playing (not on pause)
    if (isVideoOpened && _isPlaying) {
        frameTimer->start();
    }

}

// Same as with Pixel-to-Real above.
void IndoorPositioningSystemViewModel::loadIntrinsicCalibrationParams(const QString& selectedFile){

    int result = -1;
    if (selectedFile.isEmpty()) {
        emit intrinsicCalibrationParamsLoaded(false, "No file was selected!");
        return;
    }

    std::string tempFileName = selectedFile.toStdString();
    cv::FileStorage fs(tempFileName, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        QString warningMessage = QString("Error opening file: %1").arg(selectedFile);
        emit intrinsicCalibrationParamsLoaded(false, warningMessage);
        toPredictionByOptical = false;
        return;
    }

    cv::Mat cameraMatrix, optimalCameraMatrix, distCoeffs;

    // Read from XML
    fs["cameraMatrix"] >> cameraMatrix;
    fs["optimalCameraMatrix"] >> optimalCameraMatrix;
    fs["distortionCoeffs"] >> distCoeffs;
    fs.release();

    if (!optimalCameraMatrix.empty() && optimalCameraMatrix.total() == 9) { // optimal matrix has 9 elements
        videoProcessor->setOptimalCameraMatrix(optimalCameraMatrix);
    } else {
        emit intrinsicCalibrationParamsLoaded(false, "Optional. Optimal camera matrix parameters were not found or are incomplete.");
    }


    if (!distCoeffs.empty() && distCoeffs.total() == 5) { // Assuming 5 distortion coefficients
        videoProcessor->setDistCoeffs(distCoeffs);
    } else {
        videoProcessor->setDistCoeffs(distCoeffs);
        emit intrinsicCalibrationParamsLoaded(false, "Optional. Distortion coefficients were not found or are incomplete.");
    }

    if (!cameraMatrix.empty() && cameraMatrix.total() == 9) { // same as optimal matrix it has 9 elements
        videoProcessor->setCameraMatrix(cameraMatrix);
        dataProcessor->setCameraMatrix(cameraMatrix);
        if (toPredictionByOptical) {
            result = videoProcessor->setPredict(toPredictionByOptical);
            if (result != -1) {
                result = dataProcessor->setPredict(toPredictionByOptical, PredictionType::PredictionByOptical);
                if (result != -1) {
                    frameQueue.clear();
                    emit modelParamsLoaded(true, "Model was loaded successfully!");
                } else {
                    emit modelParamsLoaded(false, "Failed to load model!");
                    toPredictionByOptical = false;
                }
            } else {
                emit humanDetectorNotInitialized();
            }
        }
    } else {
        toPredictionByOptical = false;
        emit intrinsicCalibrationParamsLoaded(false, "Mandatory. Camera matrix parameters were not found or are incomplete. Method cannot be started.");
        return;
    }

    if (isVideoOpened && _isPlaying) {
        frameTimer->start();
    }
}

void IndoorPositioningSystemViewModel::onDistCoeffsLoaded() {
    emit distCoeffsLoaded();
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

// ---------------------------------- Display frames ---------------------------------------------------------------
void IndoorPositioningSystemViewModel::checkForDisplay() {
    UWBVideoData data;

    if (_isPlaying && !isExportState) {
        if (frameQueue.dequeue(data)) {
            emit frameIsReady(data);
        }
    }
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

// -------------------------------- Handle Play / Pause in Video Player (GUI) -------------------------------------------------------------

void IndoorPositioningSystemViewModel::onStartTimer() {

    int totalFrames = dataProcessor->getTotalFrames();
    dataProcessor->setAnchorPositions(anchorPositions);
    isVideoOpened = true;
    _isPlaying = true;
    isExportState = false;
    emit requestProcessVideo();
    toPredictByPixelToReal = false;

    long long videoDuration = dataProcessor->getVideoTimestampById(totalFrames - 1) - dataProcessor->getVideoTimestampById(0);
    emit videoOpened(totalFrames, videoDuration);

    if (isVideoOpened && _isPlaying) {
        frameTimer->start();
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

// ------------------------ Frame seeking -----------------------------------------------

void IndoorPositioningSystemViewModel::seekToFrame(){
    videoProcessor->pauseProcessing();
    frameQueue.clear();
    videoProcessor->seekToFrame(seekPosition);
    videoProcessor->resumeProcessing();
}

void IndoorPositioningSystemViewModel::afterSeeking() {
    if (_isPlaying) {
        frameTimer->start();
    }
}

// --------------------------------------------------- Export of data ---------------------------------------------------------------------------------
void IndoorPositioningSystemViewModel::setFrameByFrameExportRange(const QTime& startTime, const QTime& endTime) {
    long long startTimeSec = (startTime.hour() * 3600 + startTime.minute() * 60 + startTime.second());
    long long endTimeSec = (endTime.hour() * 3600 + endTime.minute() * 60 + endTime.second());

    long long startTimestampMS = startTimeSec * 1000 + dataProcessor->getVideoTimestampById(0);
    long long endTimestampMS = endTimeSec * 1000 + dataProcessor->getVideoTimestampById(0);

    // OpenCV read frames by ID. These IDs correspond to IDs stored in the video_timestamps.txt, which identify frames recorded by OpenCV.
    // These IDs are read into VideoData variable together with timestamps form video_timestamps.txt.
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

// Export only a specific frame from each segment. Segment corresponds to a stationary period in video recording. This is usefull for faster data analysis.
void IndoorPositioningSystemViewModel::onSegmentFramesExport() {
    std::vector<int> frameRangeToExport = dataProcessor->getSegmentFrameIDs();
    if (frameRangeToExport.size()) {
        setupExportConfiguration(frameRangeToExport, ExportType::SegmentFramesExport);
    } else {
        emit showExportWarning("Warning! Wrong time range.", "Time range is set wrong. Please set segment frame IDs to export.", ExportType::SegmentFramesExport);
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

// --------------------------------------------------- Handle prediction ----------------------------------------------
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
        bool toPredict = toPredictionByOptical || toPredictByPixelToReal;
        result = videoProcessor->setPredict(toPredict);
        if (result != -1) {
            result = dataProcessor->setPredict(toPredictByPixelToReal, type);
            if (result != -1) {
                frameQueue.clear();
            } else if (result == -1) {
                frameTimer->stop();
                emit modelNotLoaded(type);
                toPredictByPixelToReal = false;
                frameTimer->start();
            }
        } else {
            toPredictByPixelToReal = false;
            emit humanDetectorNotInitialized();
        }
    } else if (type == PredictionType::PredictionByOptical) {
        toPredictionByOptical = !toPredictionByOptical;
        bool toPredict = toPredictionByOptical || toPredictByPixelToReal;
        result = videoProcessor->setPredict(toPredict);
        if (result != -1) {
            result = dataProcessor->setPredict(toPredictionByOptical, type);
            if (result != -1) {
                frameQueue.clear();
            } else if (result == -1) {
                frameTimer->stop();
                emit modelNotLoaded(type);
                toPredictionByOptical = false;
                frameTimer->start();
            }
        } else {
            toPredictionByOptical = false;
            emit humanDetectorNotInitialized();
        }

    }
    videoProcessor->resumeProcessing();
}



// ----------------------------------------- Communication with Analysis Window ----------------------------------------------------------------

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

bool IndoorPositioningSystemViewModel::addAnchorPosition(int anchorID, qreal x, qreal y, bool isOrigin) {

    if (isOrigin && isOriginExists()) {
        emit showWarning("Invalid Input", "There is already an anchor set as the origin. Only one origin is allowed.");
        return false;
    }

    auto anchor = std::find_if(anchorPositions.begin(), anchorPositions.end(), [anchorID](const AnchorPosition& pos) {
        return pos.anchorID == anchorID;
    });

    if (anchor != anchorPositions.end()) {
        anchor->anchorID = anchorID;
        anchor->x = x;
        anchor->y = y;
        anchor->isOrigin = isOrigin;
        emit showWarning("Updated Anchor", "Anchor already exists. Data for anchor were re-written.");
    } else {
        AnchorPosition anchorPosition = {anchorID, x, y, isOrigin};
        anchorPositions.push_back(anchorPosition);
    }
    return true;

}

bool IndoorPositioningSystemViewModel::isOriginExists() const {
    for (const AnchorPosition& anchor : anchorPositions) {
        if (anchor.isOrigin) {
            return true;
        }
    }
    return false;
}

const std::vector<AnchorPosition>& IndoorPositioningSystemViewModel::getAnchorPositions() {
    return anchorPositions;
}

void IndoorPositioningSystemViewModel::unsetAnchorOrigin() {
    for (AnchorPosition& anchor: anchorPositions) {
        anchor.isOrigin = false;
    }
}


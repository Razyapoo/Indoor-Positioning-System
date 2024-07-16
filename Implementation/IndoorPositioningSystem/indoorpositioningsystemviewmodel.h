#ifndef INDOORPOSITIONINGSYSTEMVIEWMODEL_H
#define INDOORPOSITIONINGSYSTEMVIEWMODEL_H
/*********************************************** View Model *************************************************************
 * This class is used as a middle layer between GUI and Video Processor / Data Processor
 * It allows to eliminate direct communication between GUI and rest of the program
 * Insipred by MVVM architecture
*************************************************************************************************************************/

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QRegularExpression>
#include <opencv2/opencv.hpp>

#include "videoprocessor.h"
#include "structures.h"
#include "threadsafequeue.h"
#include "dataprocessor.h"

class IndoorPositioningSystemViewModel : public QObject
{
    Q_OBJECT

public:
    explicit IndoorPositioningSystemViewModel(QObject *parent = nullptr);
    ~IndoorPositioningSystemViewModel();

    // Load files
    bool openVideo(const QString& directory);
    void loadPixelToRealModelParams(const QString& selectedFile);
    void loadIntrinsicCalibrationParams(const QString& selectedFile);
    void loadHumanDetectorWeights(const QString& directory);

    // handle Play / Pause
    bool isPlaying() const;
    void play();
    void pause();
    void stopTimer();
    void startTimer();

    // Frame seeking
    void updateVideoPosition(int position);
    void seekToFrame();

    // Export
    void setFrameByFrameExportRange(const QTime& startTime, const QTime& endTime);
    void stopExport();

    // Handle prediction
    void setPredictByPixelToReal(bool toPredict);
    void setPredictionByOptical(bool toPredict);
    void predict(PredictionType type);

    // Handle anchors for positioning
    bool addAnchorPosition(int anchorID, qreal x, qreal y, bool isOrigin);
    const std::vector<AnchorPosition>& getAnchorPositions();
    bool isOriginExists() const;
    void unsetAnchorOrigin();

public slots:
    void onStartTimer();
    void checkForDisplay();
    void updateDataDisplay(const UWBVideoData& data);

    void afterSeeking();

    void onExportFinished(bool success);
    void onExportProgressUpdated(int index);
    void onSegmentFramesExport();

    void onChangePredictionButtonName(PredictionType type, bool isPredictionRequested);
    void onHumanDetectorNotInitialized();
    void onDistCoeffsLoaded();

    // DataAnalysisWindow
    void setRangeForDataAnalysis(const long long startTimeSec, const long long endTimeSec);
    void collectDataForPlotDistancesVsTimestamps(const int anchorID);
    void calculateRollingDeviation(const int windowSize);
    void splitDataset(const double threshold);
    void calculatePolynomialRegression(const std::vector<double>& referenceValues);
    void updateOriginalWithAdjustedValues();
    void collectDataForTag(const QString &tagIDText);

    void showAvailableTags(const std::vector<int>& availableTagIDs);
    void showAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void showPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void showPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void showDatasetSegments(const std::vector<double>& datasetSegmentMeans);
    void showOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);   


signals:
    void videoOpened(int totalFrames, long long videoDuration);
    void modelParamsLoaded(bool success, const QString& message);
    void intrinsicCalibrationParamsLoaded(bool success, const QString& message);
    void distCoeffsLoaded();
    void weightsLoaded(bool success, const QString& message);
    void modelNotLoaded(PredictionType type);
    void humanDetectorNotInitialized();

    void requestProcessVideo();
    void frameIsReady(const UWBVideoData& data);
    void dataUpdated(const QImage& image, int frameID, const QString& timestamp);
    void uwbDataUpdated(UWBData tag);
    void finishedVideoProcessing();

    void updateTagPosition(const QPointF& coordinates, int tagID);
    void updatePixelToRealPosition(const QPointF& coordinates, int objectID);
    void updateOpticalPosition(const QPointF& coordinates, int objectID);

    void positionUpdated(const QString& currentTime);
    void durationUpdated(int frameID, long long currentTimeInMSeconds);


    void showExportWarning(const QString& header, const QString& message, ExportType type);
    void showExportProcessDialog();
    void updateExportProgress(int proportion);
    void exportFinished(bool success);

    void showWarning(const QString& header, const QString& message);
    void requestChangePredictionButtonName(PredictionType type, bool isPredictionRequested);


    // DataAnalysisWindow
    void requestSetRangeForDataAnalysis(const long long startTimeSec, const long long endTimeSec);
    void requestCollectDataForPlotDistancesVsTimestamps(const int anchorID);
    void requestCalculateRollingDeviation(const int windowSize);
    void requestSplitDataset(const double threshold);
    void requestCalculatePolynomialRegression(const std::vector<double>& referenceValues);
    void requestUpdateOriginalWithAdjustedValues();
    void requestCollectDataForTag(const QString &tagIDText);

    void requestShowAvailableTags(const std::vector<int>& availableTagIDs);
    void requestShowAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void requestShowPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void requestShowPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void requestShowDatasetSegments(const std::vector<double>& segmentMeans);
    void requestShowOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);




private:
    std::unique_ptr<DataProcessor> dataProcessor;
    std::unique_ptr<VideoProcessor> videoProcessor;
    ThreadSafeQueue frameQueue; // frames to show
    QTimer* frameTimer; // handle fps of Video Player

    bool isVideoOpened;
    std::vector<AnchorPosition> anchorPositions;

    int seekPosition, lastPosition;
    double fps;
    int totalFrames;
    bool _isPlaying, isExportState;

    bool toPredictByPixelToReal, toPredictionByOptical;
    int totalExportDuration;

    // Data Analysis
    bool setDataAnalysisTimeStart, setDataAnalysisTimeEnd;
    long long dataAnalysisTimeStart, dataAnalysisTimeEnd;

    void setupExportConfiguration(const std::vector<int>& frameRangeToExport, ExportType type);
};

#endif // INDOORPOSITIONINGSYSTEMVIEWMODEL_H

#ifndef INDOORPOSITIONINGSYSTEMVIEWMODEL_H
#define INDOORPOSITIONINGSYSTEMVIEWMODEL_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtMultimedia/QtMultimedia>
#include <QtWidgets>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QImage>
#include <QThread>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QProgressDialog>
#include <QFile>
#include <QIODevice>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>

#include <QLabel>


#include <memory>
#include <QException>

#include "videoprocessor.h"
#include "structures.h"
#include "threadsafequeue.h"
#include "dataprocessor.h"
#include "customlabel.h"

class IndoorPositioningSystemViewModel : public QObject
{
    Q_OBJECT

public:
    explicit IndoorPositioningSystemViewModel(QObject *parent = nullptr);
    ~IndoorPositioningSystemViewModel();

    bool isPlayingCheck() const;
    void play();
    void pause();
    void stopTimer();
    void startTimer();
    void openVideo(const QString& directory);
    void loadModelParams(const QString& selectedFile);
    void loadIntrinsicCalibrationParams(const QString& selectedFile);
    void updateVideoPosition(int position);
    void seekToFrame();
    void setFrameByFrameExportRange(const QTime& startTime, const QTime& endTime);
    void stopExport();
    void setPredictByModel(bool toPredict);
    void setPredictByHeight(bool toPredict);
    void predict(PredictionType type);


public slots:
    void checkForDisplay();
    void updateDataDisplay(const UWBVideoData& data);
    void onExportFinished(bool success);
    void onExportProgressUpdated(int index);
    void onSegmentFramesExport();
    void afterSeeking();

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
    void requestProcessVideo();
    void frameIsReady(const UWBVideoData& data);
    void dataUpdated(const QImage& image, int frameID, const QString& timestamp);
    void uwbDataUpdated(int tagID, const QString& timestamp, const QString& distanceAnchor1, const QString& distanceAnchor2);
    void finishedVideoProcessing();
    void updateTagPosition(const QPointF& coordinates, int tagID);
    void durationUpdated(int frameID, long long currentTimeInMSeconds);
    void videoOpened(int totalFrames, long long videoDuration);
    void videoOpenFailed(bool success, const QString& message);
    void modelParamsLoaded(bool success, const QString& message);
    void intrinsicCalibrationParamsLoaded(bool success, const QString& message);
    void showExportWarning(const QString& header, const QString& message, ExportType type);
    void showExportProcessDialog();
    void updateExportProgress(int proportion);
    void exportFinished(bool success);
    void modelNotLoaded(PredictionType type);
    void positionUpdated(const QString& currentTime);
    void showWarning(const QString& header, const QString& message);


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

    QTimer* frameTimer;
    bool isVideoOpened;

    int seekPosition, lastPosition;
    double fps;
    int totalFrames;
    bool isPlaying, isExportState;

    ThreadSafeQueue frameQueue;
    bool toPredictByModel, toPredictByHeight;
    int totalExportDuration;
    void setupExportConfiguration(const std::vector<int>& frameRangeToExport, ExportType type);


    // Data Analysis
    bool setDataAnalysisTimeStart, setDataAnalysisTimeEnd;
    long long dataAnalysisTimeStart, dataAnalysisTimeEnd;
};

#endif // INDOORPOSITIONINGSYSTEMVIEWMODEL_H

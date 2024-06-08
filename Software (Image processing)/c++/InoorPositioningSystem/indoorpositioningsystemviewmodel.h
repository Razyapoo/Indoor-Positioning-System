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
#include "dataanalysiswindow.h"
#include "uwblocalizationwindow.h"
#include "exporttimerangesetter.h"
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


private slots:
    void checkForDisplay();
    void updateDataDisplay(const UWBVideoData& data);
    void onExportFinished(bool success);
    void onExportProgressUpdated(int index);
    // void onSegmentFramesExport();
    void afterSeeking();

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



private:
    std::unique_ptr<DataProcessor> dataProcessor;
    std::unique_ptr<VideoProcessor> videoProcessor;

    QTimer* frameTimer;
    bool isVideoOpened;

    int seekPosition, lastPosition;
    // double videoDuration;
    double fps;
    int totalFrames;
    bool isPlaying, isExportState;

    ThreadSafeQueue frameQueue;
    // std::thread checkForDisplayThread;


    bool toPredictByModel, toPredictByHeight;
    int totalExportDuration;


    // void setDuration(qint64 duration);
    void setupExportConfiguration(const std::vector<int>& frameRangeToExport, ExportType type);


    // Data Analysis
    bool setDataAnalysisTimeStart, setDataAnalysisTimeEnd;
    long long dataAnalysisTimeStart, dataAnalysisTimeEnd;
};

#endif // INDOORPOSITIONINGSYSTEMVIEWMODEL_H

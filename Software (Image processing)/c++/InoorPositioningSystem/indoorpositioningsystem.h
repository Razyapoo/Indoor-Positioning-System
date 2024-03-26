#ifndef INDOORPOSITIONINGSYSTEM_H
#define INDOORPOSITIONINGSYSTEM_H

#include <QMainWindow>
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

QT_BEGIN_NAMESPACE
namespace Ui {
class IndoorPositioningSystem;
}
QT_END_NAMESPACE

class IndoorPositioningSystem : public QMainWindow
{
    Q_OBJECT

public:
    IndoorPositioningSystem(QWidget *parent = nullptr);
    ~IndoorPositioningSystem();

    void loadVideo(const std::string& filename);

private slots:
    void updateDataDisplay(const UWBVideoData& data);
    void on_horizontalSlider_Duration_valueChanged(int position);

    void on_pushButton_Play_Pause_clicked();
    void on_actionOpen_Video_triggered();

    // void stopCheckingForDisplayThread();

    // void on_horizontalSlider_Duration_sliderPressed();
    void on_horizontalSlider_Duration_sliderReleased();
    void afterSeeking();
    // void seekToFrame();
    // void startProcessVideo();

    void on_pushButton_UWB_Data_Analysis_clicked();

    void on_pushButton_UWB_Localization_clicked();
    // void onUWBLocalizationWindowClosed();

    void on_pushButton_Export_Data_clicked();
    void onAcceptFrameByFrameExport();
    void onSegmentFramesExport();
    void onExportFinish(bool success);
    // void onExportProgressUpdated(int frameIndex);

    void on_actionLoad_params_triggered();

    void on_pushButton_Polyn_Regression_Predict_clicked();

signals:
    void frameIsReady(const UWBVideoData& data);
    // void requestStopProcessing(bool isExportState = false);
    // void requestContinueProcessing();
    // void requestSeekToFrame(int position);
    void requestProcessVideo();
    void requestLoadData(const std::string& UWBDataFilename, const std::string& videoDataFilename);
    void requestVideoInit(const std::string& filename);
    void finishedVideoProcessing();
    void tagPositionUpdated(const QPointF& position, int tagID);
    // void requestDataExport(int endPosition);

private:
    Ui::IndoorPositioningSystem *ui;
    std::unique_ptr<DataAnalysisWindow> dataAnalysisWindow;
    std::unique_ptr<UWBLocalizationWindow> uwbLocalizationWindow;
    std::unique_ptr<ExportTimeRangeSetter> exportTimeRangeSetter;
    std::unique_ptr<VideoProcessor> videoProcessor;
    std::unique_ptr<DataProcessor> dataProcessor;
    QTimer* frameTimer;
    // VideoProcessor* videoProcessor;
    // DataProcessor* dataProcessor;
    // QTimer* frameTimer;
    QPixmap qPixmap;
    QLayout* uwbContainerLayout;


    int seekPosition, lastPosition;
    // double videoDuration;
    double fps;
    int totalFrames;
    bool isPause, isExportState;
    // std::atomic<bool> toShowUWBLocalization;

    // QThread videoThread;
    // QThread uwbDataThread;
    // QThread dataAnalysisWindowThread;
    ThreadSafeQueue frameQueue;
    // std::thread checkForDisplayThread;

    QProgressDialog* exportProgressDialog;



    void checkForDisplay();
    void setDuration(qint64 duration);
    void setupExportConfiguration(const std::vector<int>& segmentFrameIDs, ExportType type);


    // Data Analysis
    bool setDataAnalysisTimeStart, setDataAnalysisTimeEnd;
    long long dataAnalysisTimeStart, dataAnalysisTimeEnd;

    // int startDataExportPosition;
    // int endDataExportPosition;

};
#endif // INDOORPOSITIONINGSYSTEM_H

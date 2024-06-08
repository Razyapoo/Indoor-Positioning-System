#ifndef IndoorPositioningSystemUI_H
#define IndoorPositioningSystemUI_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtMultimedia/QtMultimedia>
#include <QtWidgets>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QImage>
// #include <QThread>
// #include <QTimer>
// #include <QDir>
#include <QDebug>
#include <QLayout>
#include <QLabel>
// #include <QProgressDialog>
#include <QFile>
#include <QIODevice>
// #include <QXmlStreamReader>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>

// #include <QLabel>


// #include <memory>
#include <QException>

// #include "videoprocessor.h"
// #include "structures.h"
// #include "threadsafequeue.h"
// #include "dataprocessor.h"
#include "dataanalysiswindow.h"
#include "uwblocalizationwindow.h"
#include "exporttimerangesetter.h"
#include "customlabel.h"
#include "indoorpositioningsystemviewmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class IndoorPositioningSystemUI;
}
QT_END_NAMESPACE

class IndoorPositioningSystemUI : public QMainWindow
{
    Q_OBJECT

public:
    IndoorPositioningSystemUI(QWidget *parent = nullptr);
    ~IndoorPositioningSystemUI();

    // void loadVideo(const std::string& filename);

private slots:
    // void updateDataDisplay(const UWBVideoData& data);
    void on_horizontalSlider_Duration_valueChanged(int position);

    void on_pushButton_Play_Pause_clicked();
    void on_actionOpen_Video_triggered();

    // // void stopCheckingForDisplayThread();

    // // void on_horizontalSlider_Duration_sliderPressed();
    void on_horizontalSlider_Duration_sliderReleased();
    // void afterSeeking();
    // // void seekToFrame();
    // // void startProcessVideo();

    void on_pushButton_UWB_Data_Analysis_clicked();

    void on_pushButton_UWB_Localization_clicked();
    // // void onUWBLocalizationWindowClosed();

    void on_pushButton_Export_Data_clicked();
    void onAcceptFrameByFrameExport();
    // void onSegmentFramesExport();
    // void onExportFinish(bool success);
    // // void onExportProgressUpdated(int frameIndex);

    void on_actionLoad_model_params_triggered();

    void on_pushButton_Predict_Optical_clicked();

    void on_pushButton_Predict_Model_clicked();

    void on_actionLoad_intrinsic_calibration_params_triggered();

    void onDataUpdated(const QImage& image, int frameID, const QString& timestamp);
    void onUWBDataUpdated(int tagID, const QString& timestamp, const QString& distanceAnchor1, const QString& distanceAnchor2);
    void onFinishedVideoProcessing();
    void onDurationUpdated(int frameID, long long currentTimeInMSeconds);
    void onVideoOpened(int totalFrames, long long videoDuration);
    void onFileLoaded(bool success, const QString& message);
    void onShowExportWarning(const QString& header, const QString& message, ExportType type);
    void onShowExportProcessDialog();
    void onUpdateExportProgress(int proportion);
    void onExportFinished(bool success);
    void onModelNotLoaded(PredictionType type);
    void onPositionUpdated(const QString& currentTime);
    void onShowWarning(const QString& header, const QString& message);

signals:
    // void frameIsReady(const UWBVideoData& data);
    // void requestProcessVideo();
    // void requestLoadData(const std::string& UWBDataFilename, const std::string& videoDataFilename);
    // void requestVideoInit(const std::string& filename);
    // void finishedVideoProcessing();
    void tagPositionUpdated(const QPointF& position, int tagID);

private:
    Ui::IndoorPositioningSystemUI *ui;
    std::unique_ptr<IndoorPositioningSystemViewModel> viewModel;

    QPixmap qPixmap;

    std::unique_ptr<DataAnalysisWindow> dataAnalysisWindow;
    std::unique_ptr<UWBLocalizationWindow> uwbLocalizationWindow;
    std::unique_ptr<ExportTimeRangeSetter> exportTimeRangeSetter;
    QProgressDialog* exportProgressDialog;

    void loadModelParams();
    void loadIntrinsicCalibrationParams();

};
#endif // IndoorPositioningSystemUI_H

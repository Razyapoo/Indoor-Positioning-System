#ifndef IndoorPositioningSystemUI_H
#define IndoorPositioningSystemUI_H

/*********************************************** Main GUI (Video Player) *************************************************
 * This is a Main Video Player which shows synchronized Video and estimated data (UWB, Pixel-to-Real, Optical)
 * It communicates with other part of the program though ViewModel class
 *
 * Responsible for:
 *  - Visualization of main functions (buttons, video, uwb data etc.)
 *  - Load / Play / Pause / Seek data stream
 *  - Load addiotnal data for HumanDetector (.weights, .cfg), XGBoost (.json), Intrinsic Camera Parameters (.xml)
 *
 * Folder structure to be opened in Video Pleayer as a project (raw video + raw UWB data):
 *  - UWB_timestamps.txt
 *  - video.avi
 *  - video_timestamps.txt
*************************************************************************************************************************/

#include <QMainWindow>
#include <QImage>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QException>
#include <QProgressDialog>

#include "structures.h"
#include "dataanalysiswindow.h"
#include "uwblocalizationwindow.h"
#include "anchorinputwindow.h"
#include "exporttimerangesetter.h"
#include "coordinateswindow.h"
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

private slots:
    // Handle load actions
    void on_actionOpen_Video_triggered();
    void on_actionLoad_intrinsic_calibration_params_triggered();
    void on_actionLoad_model_params_triggered();
    void on_actionLoad_human_detector_weights_triggered();

    // After loading data
    void onVideoOpened(int totalFrames, long long videoDuration);
    void onFileLoaded(bool success, const QString& message);
    void onDistCoeffsLoaded();

    void onModelNotLoaded(PredictionType type);
    void onHumanDetectorNotInitialized();

    // Update Data in GUI
    void onUWBDataUpdated(UWBData tag);
    void onDataUpdated(const QImage& image, int frameID, const QString& timestamp);
    void onPositionUpdated(const QString& currentTime);
    void onDurationUpdated(int frameID, long long currentTimeInMSeconds);
    void onShowWarning(const QString& header, const QString& message);
    void onFinishedVideoProcessing();

    // Handle export
    void onAcceptFrameByFrameExport();
    void onShowExportWarning(const QString& header, const QString& message, ExportType type);
    void onShowExportProcessDialog();
    void onUpdateExportProgress(int proportion);
    void onExportFinished(bool success);

    void onChangePredictionButtonName(PredictionType type, bool isPredictionRequested);

    // Handle events
    void on_horizontalSlider_Duration_sliderReleased();
    void on_horizontalSlider_Duration_valueChanged(int position);

    void on_pushButton_Play_Pause_clicked();
    void on_pushButton_UWB_Data_Analysis_clicked();
    void on_pushButton_UWB_Localization_clicked();
    void on_pushButton_Export_Data_clicked();
    void on_pushButton_Predict_Optical_clicked();
    void on_pushButton_Predict_Model_clicked();
    void on_pushButton_UWB_Show_Coordinates_clicked();
    void on_pushButton_Optical_Show_Coordinates_clicked();
    void on_pushButton_Pixel_to_Real_Show_Coordinates_clicked();

private:
    Ui::IndoorPositioningSystemUI *ui;
    std::unique_ptr<IndoorPositioningSystemViewModel> viewModel;

    QPixmap qPixmap; // necessary format of image for visualization

    std::unique_ptr<DataAnalysisWindow> dataAnalysisWindow;
    std::unique_ptr<UWBLocalizationWindow> uwbLocalizationWindow;
    std::unique_ptr<ExportTimeRangeSetter> exportTimeRangeSetter;
    std::unique_ptr<CoordinatesWindow> uwbCoordinatesWindow;
    std::unique_ptr<CoordinatesWindow> pixelToRealCoordinatesWindow;
    std::unique_ptr<CoordinatesWindow> opticalCoordinatesWindow;
    std::unique_ptr<AnchorInputWindow> anchorInputWindow;

    QProgressDialog* exportProgressDialog;

    void loadPixelToRealModelParams();
    void loadIntrinsicCalibrationParams();
    void loadHumanDetectorWeights();

};
#endif

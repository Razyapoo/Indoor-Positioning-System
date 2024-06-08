#include "indoorpositioningsystemui.h"
#include "./ui_indoorpositioningsystemui.h"

IndoorPositioningSystemUI::IndoorPositioningSystemUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IndoorPositioningSystemUI)

{
    ui->setupUi(this);

    setWindowTitle("Indoor Positioning System");

    // Layout organization
    // mainLayout = new QHBoxLayout(this);
    setFixedSize(1720, 980);
    viewModel = std::make_unique<IndoorPositioningSystemViewModel>(this);
    exportProgressDialog = nullptr;




    // frameTimer = new QTimer(this);
    // // frameTimer = std::make_unique<QTimer>(this);
    // frameTimer->setInterval(60); // 3500 for human detector

    // dataProcessor = std::make_unique<DataProcessor>(frameQueue);
    // videoProcessor = std::make_unique<VideoProcessor>(frameQueue, dataProcessor.get());
    // exportProgressDialog = nullptr;


    // connect(frameTimer, &QTimer::timeout, this, &IndoorPositioningSystemUI::checkForDisplay);
    // connect(this, &IndoorPositioningSystemUI::frameIsReady, this, &IndoorPositioningSystemUI::updateDataDisplay);
    // connect(this, &IndoorPositioningSystemUI::requestProcessVideo, videoProcessor.get(), &VideoProcessor::processVideo);

    // connect(this, &IndoorPositioningSystemUI::requestStopProcessing, videoProcessor.get(),  &VideoProcessor::stopProcessing, Qt::DirectConnection);
    // connect(this, &IndoorPositioningSystemUI::requestContinueProcessing, videoProcessor,  &VideoProcessor::continueProcessing, Qt::BlockingQueuedConnection);
    // connect(this, &IndoorPositioningSystemUI::requestSeekToFrame, videoProcessor.get(),  &VideoProcessor::seekToFrame, Qt::DirectConnection);
    // connect(videoProcessor.get(), &VideoProcessor::processingIsStopped, this, &IndoorPositioningSystemUI::seekToFrame);
    // connect(this, &IndoorPositioningSystemUI::requestVideoInit, videoProcessor.get(), &VideoProcessor::init, Qt::BlockingQueuedConnection);
    // connect(this, &IndoorPositioningSystemUI::requestLoadData, dataProcessor.get(), &DataProcessor::loadData, Qt::BlockingQueuedConnection);

    // connect(videoProcessor.get(), &VideoProcessor::seekingDone, this, &IndoorPositioningSystemUI::afterSeeking);

    // connect(this, &IndoorPositioningSystemUI::requestDataExport, videoProcessor.get(), &VideoProcessor::dataExport, Qt::BlockingQueuedConnection);//, Qt::DirectConnection);

    // connect(this, &IndoorPositioningSystemUI::requestAnalyseData, dataProcessor.get(), &DataProcessor::dataAnalysisInit, Qt::DirectConnection);

    // connect(this, &IndoorPositioningSystemUI::finishedVideoProcessing, &videoThread, &QThread::quit);
    // connect(this, &IndoorPositioningSystemUI::finishedVideoProcessing, videoProcessor.get(), &QObject::deleteLater);

    // connect(this, &QMainWindow::destroyed, &videoThread, &QThread::quit);
    // connect(this, &QMainWindow::destroyed, videoProcessor.get(), &QObject::deleteLater);
    // connect(this, &QMainWindow::destroyed, &uwbDataThread, &QThread::quit);
    // connect(this, &QMainWindow::destroyed, dataProcessor.get(), &QObject::deleteLater);

    // connect(dataAnalysisWindow.get(), &QDialog::finished, &dataAnalysisWindowThread, &QThread::quit);
    // connect(uwbLocalizationWindow.get(), &QDialog::finished, uwbLocalizationWindow.get(), &QDialog::deleteLater);

    // connect(videoProcessor, &VideoProcessor::finished, this, &IndoorPositioningSystemUI::stopCheckingForDisplayThread);
    // connect(&videoThread, &QThread::finished, &videoThread, &QThread::deleteLater);

    // ui->pushButton_Polyn_Regression_Predict-
    // dataProcessor->moveToThread(&uwbDataThread);
    // videoProcessor->moveToThread(&videoThread);
    // ui->timeEdit_Data_Analysis_Start->setDisplayFormat("HH:mm:ss");
    // ui->timeEdit_Data_Analysis_End->setDisplayFormat("HH:mm:ss");

    // // ClickEventFilter *filter = new ClickEventFilter(this);
    // ui->timeEdit_Data_Analysis_Start->installEventFilter(this);



    connect(viewModel.get(), &IndoorPositioningSystemViewModel::dataUpdated, this, &IndoorPositioningSystemUI::onDataUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::uwbDataUpdated, this, &IndoorPositioningSystemUI::onUWBDataUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::finishedVideoProcessing, this, &IndoorPositioningSystemUI::onFinishedVideoProcessing);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::durationUpdated, this, &IndoorPositioningSystemUI::onDurationUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::videoOpened, this, &IndoorPositioningSystemUI::onVideoOpened);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::modelParamsLoaded, this, &IndoorPositioningSystemUI::onFileLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::intrinsicCalibrationParamsLoaded, this, &IndoorPositioningSystemUI::onFileLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::showExportProcessDialog, this, &IndoorPositioningSystemUI::onShowExportProcessDialog);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::showExportWarning, this, &IndoorPositioningSystemUI::onShowExportWarning);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateExportProgress, this, &IndoorPositioningSystemUI::onUpdateExportProgress);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::exportFinished, this, &IndoorPositioningSystemUI::onExportFinished);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::modelNotLoaded, this, &IndoorPositioningSystemUI::onModelNotLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::positionUpdated, this, &IndoorPositioningSystemUI::onPositionUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::showWarning, this, &IndoorPositioningSystemUI::onShowWarning);



    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));




}

IndoorPositioningSystemUI::~IndoorPositioningSystemUI()
{
    delete ui;
}

void IndoorPositioningSystemUI::onDataUpdated(const QImage &image, int frameID, const QString &timestamp){
    qPixmap = QPixmap::fromImage(image);
    qPixmap = qPixmap.scaled(ui->label_Video->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->label_Video->setPixmap(qPixmap);
    ui->label_Video_Frame_ID_value->setNum(frameID);
    ui->label_Video_Frame_Timestamp_value->setText(timestamp);
}

void IndoorPositioningSystemUI::onUWBDataUpdated(int tagID, const QString& timestamp, const QString& distanceAnchor1, const QString& distanceAnchor2)
{
    switch (tagID)
    {
    case 1:
        ui->label_Tag_ID_value_1->setNum(tagID);
        ui->label_Tag_timestamp_value_1->setText(timestamp);
        ui->label_Anchor101_value_1->setText(distanceAnchor1);
        ui->label_Anchor102_value_1->setText(distanceAnchor2);
        break;
    case 2:
        ui->label_Tag_ID_value_2->setNum(tagID);
        ui->label_Tag_timestamp_value_2->setText(timestamp);
        ui->label_Anchor101_value_2->setText(distanceAnchor1);
        ui->label_Anchor102_value_2->setText(distanceAnchor2);
        break;
    case 3:
        ui->label_Tag_ID_value_3->setNum(tagID);
        ui->label_Tag_timestamp_value_3->setText(timestamp);
        ui->label_Anchor101_value_3->setText(distanceAnchor1);
        ui->label_Anchor102_value_3->setText(distanceAnchor2);
        break;
    }
}

void IndoorPositioningSystemUI::onDurationUpdated(int frameID, long long currentTimeInMSeconds)
{
    if (!ui->horizontalSlider_Duration->isSliderDown()) {
        QTime currentTime = QTime(0, 0).addMSecs(static_cast<int>(currentTimeInMSeconds));
        ui->horizontalSlider_Duration->setValue(frameID);
        ui->label_Current_Time->setText(currentTime.toString("HH:mm:ss"));
    }
}

void IndoorPositioningSystemUI::on_pushButton_Play_Pause_clicked()
{
    if (viewModel->isPlayingCheck()) {
        viewModel->pause();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    } else {
        viewModel->play();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }

}

void IndoorPositioningSystemUI::on_actionOpen_Video_triggered()
{
    viewModel->stopTimer();
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", (QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));

    if (!directory.isEmpty()) {
        viewModel->openVideo(directory);
    } else {
        QMessageBox::warning(this, "Warning", "No directory was selected!");
    }
}

void IndoorPositioningSystemUI::onVideoOpened(int totalFrames, long long videoDuration)
{
    ui->horizontalSlider_Duration->setRange(1, totalFrames);
    QTime totalTime = QTime(0, 0).addMSecs(static_cast<int>(videoDuration));
    ui->label_Total_Time->setText(totalTime.toString("HH:mm:ss"));
    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

void IndoorPositioningSystemUI::loadModelParams() {
    viewModel->stopTimer();
    QString selectedFile = QFileDialog::getOpenFileName(this, "Select File", QDir::homePath(), "All Supported Files (*.model *.json);;XGBoost Model Files (*.model);;JSON Files (*.json)");

    if (!selectedFile.isEmpty()) {
        viewModel->loadModelParams(selectedFile);
    } else {
        QMessageBox::warning(this, "Warning", "No file was selected!");
    }
}

void IndoorPositioningSystemUI::on_actionLoad_model_params_triggered()
{
    loadModelParams();
}

void IndoorPositioningSystemUI::onFileLoaded(bool success, const QString& message)
{
    if (success) {
        QMessageBox::information(this, "Success", message);
    } else {
        QMessageBox::critical(this, "Error", message);
    }
}

void IndoorPositioningSystemUI::on_actionLoad_intrinsic_calibration_params_triggered()
{
    loadIntrinsicCalibrationParams();
}



void IndoorPositioningSystemUI::loadIntrinsicCalibrationParams() {
    viewModel->stopTimer();
    QString selectedFile = QFileDialog::getOpenFileName(this, "Select File", QDir::homePath(), "XML Files (*.xml)");

    if (!selectedFile.isEmpty()) {
        viewModel->loadIntrinsicCalibrationParams(selectedFile);
    } else {
        QMessageBox::warning(this, "Warning", "No file was selected!");
    }
}

void IndoorPositioningSystemUI::on_horizontalSlider_Duration_valueChanged(int position) {
    if (ui->horizontalSlider_Duration->isSliderDown()) {
        viewModel->updateVideoPosition(position);
    }
}

void IndoorPositioningSystemUI::onPositionUpdated(const QString &currentTime) {
    ui->label_Current_Time->setText(currentTime);
}

void IndoorPositioningSystemUI::on_horizontalSlider_Duration_sliderReleased()
{
    viewModel->seekToFrame();
}

void IndoorPositioningSystemUI::on_pushButton_UWB_Data_Analysis_clicked()
{
    // if (!dataAnalysisWindow) {
    //     dataAnalysisWindow = std::make_unique<DataAnalysisWindow>(this, viewModel->dataProcessor.get(), fps);
    //     connect(dataAnalysisWindow.get(), &DataAnalysisWindow::requestSegmentFramesExport, this, &IndoorPositioningSystemViewModel::onSegmentFramesExport);
    // }

    // dataAnalysisWindow->show();
}

void IndoorPositioningSystemUI::on_pushButton_UWB_Localization_clicked()
{
    if (!uwbLocalizationWindow) {
        std::vector<QPointF> anchorPositions = {QPointF(0.627, 0), QPointF(3.127, 0)};

        uwbLocalizationWindow = std::make_unique<UWBLocalizationWindow>(this, anchorPositions);
        connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateTagPosition, uwbLocalizationWindow.get(), &UWBLocalizationWindow::updateTagPosition);
    }

    uwbLocalizationWindow->show();
}


void IndoorPositioningSystemUI::on_pushButton_Export_Data_clicked()
{

    if (!exportTimeRangeSetter) {
        exportTimeRangeSetter = std::make_unique<ExportTimeRangeSetter>(this);

        connect(exportTimeRangeSetter.get(), &QDialog::accepted, this, &IndoorPositioningSystemUI::onAcceptFrameByFrameExport);
    }

    exportTimeRangeSetter->startTimeEdit->setTime(QTime(0, 0));
    exportTimeRangeSetter->endTimeEdit->setTime(QTime(0, 0));
    exportTimeRangeSetter->show();

}

void IndoorPositioningSystemUI::onShowExportProcessDialog() {
    if (!exportProgressDialog) {
        exportProgressDialog = new QProgressDialog("Exporting data...", "Cancel", 0, 100, this); // 0..100%
        exportProgressDialog->setWindowModality(Qt::WindowModal);

        connect(exportProgressDialog, &QProgressDialog::canceled, this, [this]() {
            viewModel->stopExport();
        });
    }

    exportProgressDialog->show();
}

void IndoorPositioningSystemUI::onAcceptFrameByFrameExport() {
    ui->horizontalSlider_Duration->setEnabled(false);
    ui->pushButton_Export_Data->setEnabled(false);
    ui->pushButton_UWB_Data_Analysis->setEnabled(false);
    ui->pushButton_UWB_Localization->setEnabled(false);
    ui->pushButton_Play_Pause->setEnabled(false);

    QTime startTime = exportTimeRangeSetter->startTimeEdit->time();
    QTime endTime = exportTimeRangeSetter->endTimeEdit->time();

    viewModel->setFrameByFrameExportRange(startTime, endTime);
}

void IndoorPositioningSystemUI::onShowExportWarning(const QString& header, const QString& message, ExportType type)
{
    if (type == ExportType::FrameByFrameExport) {
        exportTimeRangeSetter->startTimeEdit->setTime(QTime(0, 0));
        exportTimeRangeSetter->endTimeEdit->setTime(QTime(0, 0));
        exportTimeRangeSetter->show();
    }

    QMessageBox::warning(this, header, message);
}

void IndoorPositioningSystemUI::onUpdateExportProgress(int proportion) {
    exportProgressDialog->setValue(proportion);
}

void IndoorPositioningSystemUI::onExportFinished(bool success) {
    if (success) {
        exportProgressDialog->setValue(exportProgressDialog->maximum());
        QMessageBox::information(this, "Export Complete", "The data export operation has completed successfully.");
    } else {
        exportProgressDialog->reset();
    }

    ui->horizontalSlider_Duration->setEnabled(true);
    ui->pushButton_Export_Data->setEnabled(true);
    ui->pushButton_UWB_Data_Analysis->setEnabled(true);
    ui->pushButton_UWB_Localization->setEnabled(true);
    ui->pushButton_Play_Pause->setEnabled(true);
}

void IndoorPositioningSystemUI::on_pushButton_Predict_Model_clicked()
{
    viewModel->predict(PredictionType::PredictionByModel);
}


void IndoorPositioningSystemUI::on_pushButton_Predict_Optical_clicked()
{
   viewModel->predict(PredictionType::PredictionByHeight);
}

void IndoorPositioningSystemUI::onModelNotLoaded(PredictionType type) {
    QMessageBox::StandardButton reply;

    if (type == PredictionType::PredictionByModel) {
        reply = QMessageBox::question(this, "Prediction Model", "XGBoost prediction model is not loaded. Do you want to load one?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            loadModelParams();
        } else {
            viewModel->setPredictByModel(false);
        }
    } else  if (type == PredictionType::PredictionByHeight) {
        reply = QMessageBox::question(this, "Camera Calibration Parameters", "The camera calibration parameters are not loaded. Do you want to load one?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            loadIntrinsicCalibrationParams();
        } else {
            viewModel->setPredictByHeight(false);
        }
    }
}

void IndoorPositioningSystemUI::onFinishedVideoProcessing() {
    QMessageBox::information(this, "Finish", "Video processing has completed successfully.");
}

void IndoorPositioningSystemUI::onShowWarning(const QString& header, const QString &message) {
    QMessageBox::warning(this, header, message);
}



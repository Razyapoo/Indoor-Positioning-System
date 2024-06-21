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
    setFixedSize(1720, 1000);
    viewModel = std::make_unique<IndoorPositioningSystemViewModel>(this);
    exportProgressDialog = nullptr;

    connect(viewModel.get(), &IndoorPositioningSystemViewModel::dataUpdated, this, &IndoorPositioningSystemUI::onDataUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::uwbDataUpdated, this, &IndoorPositioningSystemUI::onUWBDataUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::finishedVideoProcessing, this, &IndoorPositioningSystemUI::onFinishedVideoProcessing);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::durationUpdated, this, &IndoorPositioningSystemUI::onDurationUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::videoOpened, this, &IndoorPositioningSystemUI::onVideoOpened);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::modelParamsLoaded, this, &IndoorPositioningSystemUI::onFileLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::weightsLoaded, this, &IndoorPositioningSystemUI::onFileLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::intrinsicCalibrationParamsLoaded, this, &IndoorPositioningSystemUI::onFileLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::showExportProcessDialog, this, &IndoorPositioningSystemUI::onShowExportProcessDialog);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::showExportWarning, this, &IndoorPositioningSystemUI::onShowExportWarning);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateExportProgress, this, &IndoorPositioningSystemUI::onUpdateExportProgress);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::exportFinished, this, &IndoorPositioningSystemUI::onExportFinished);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::modelNotLoaded, this, &IndoorPositioningSystemUI::onModelNotLoaded);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::positionUpdated, this, &IndoorPositioningSystemUI::onPositionUpdated);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::showWarning, this, &IndoorPositioningSystemUI::onShowWarning);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::requestChangePredictionButtonName, this, &IndoorPositioningSystemUI::onChangePredictionButtonName);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::humanDetectorNotInitialized, this, &IndoorPositioningSystemUI::onHumanDetectorNotInitialized);
    connect(viewModel.get(), &IndoorPositioningSystemViewModel::distCoeffsLoaded, this, &IndoorPositioningSystemUI::onDistCoeffsLoaded);

    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    ui->pushButton_Play_Pause->setDisabled(true);
    ui->pushButton_Predict_Optical->setDisabled(true);
    ui->pushButton_Predict_Model->setDisabled(true);
    ui->pushButton_Export_Data->setDisabled(true);
    ui->pushButton_UWB_Localization->setDisabled(true);
    ui->pushButton_UWB_Data_Analysis->setDisabled(true);
    ui->horizontalSlider_Duration->setDisabled(true);

    ui->pushButton_Optical_Show_Coordinates->setDisabled(true);
    ui->pushButton_Pixel_to_Real_Show_Coordinates->setDisabled(true);
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

void IndoorPositioningSystemUI::onUWBDataUpdated(UWBData tag)
{
    QString tagTimestampText = QString::number(tag.timestamp);

    QString measurementTimeText;
    if (tag.measurementTime) {
        measurementTimeText = QString::number(tag.measurementTime.value()) + " ms";
    }

    switch (tag.tagID)
    {
    case 1:
        ui->label_Tag_ID_value_1->setNum(tag.tagID);
        ui->label_Tag_timestamp_value_1->setText(tagTimestampText);
        ui->label_Anchor1_ID_value_1->setNum(tag.anchorList[0].anchorID);
        ui->label_Anchor1_Distance_value_1->setNum(tag.anchorList[0].distance);
        ui->label_Anchor2_ID_value_1->setNum(tag.anchorList[1].anchorID);
        ui->label_Anchor2_Distance_value_1->setNum(tag.anchorList[1].distance);
        if (tag.measurementTime) {
            ui->label_Tag_measurement_time_value_1->setText(measurementTimeText);
        } else {
            ui->label_Tag_measurement_time_value_1->setText("N/A");
        }
        break;
    case 2:
        ui->label_Tag_ID_value_2->setNum(tag.tagID);
        ui->label_Tag_timestamp_value_2->setText(tagTimestampText);
        ui->label_Anchor1_ID_value_2->setNum(tag.anchorList[0].anchorID);
        ui->label_Anchor1_Distance_value_2->setNum(tag.anchorList[0].distance);
        ui->label_Anchor2_ID_value_2->setNum(tag.anchorList[1].anchorID);
        ui->label_Anchor2_Distance_value_2->setNum(tag.anchorList[1].distance);
        if (tag.measurementTime) {
            ui->label_Tag_measurement_time_value_2->setText(measurementTimeText);
        } else {
            ui->label_Tag_measurement_time_value_2->setText("N/A");
        }
        break;
    case 3:
        ui->label_Tag_ID_value_3->setNum(tag.tagID);
        ui->label_Tag_timestamp_value_3->setText(tagTimestampText);
        ui->label_Anchor1_ID_value_3->setNum(tag.anchorList[0].anchorID);
        ui->label_Anchor1_Distance_value_3->setNum(tag.anchorList[0].distance);
        ui->label_Anchor2_ID_value_3->setNum(tag.anchorList[1].anchorID);
        ui->label_Anchor2_Distance_value_3->setNum(tag.anchorList[1].distance);
        if (tag.measurementTime) {
            ui->label_Tag_measurement_time_value_3->setText(measurementTimeText);
        } else {
            ui->label_Tag_measurement_time_value_3->setText("N/A");
        }
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
    if (viewModel->isPlaying()) {
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
    bool isOpened = false;

    if (!directory.isEmpty()) {
        isOpened = viewModel->openVideo(directory);
    } else {
        QMessageBox::warning(this, "Warning", "No directory was selected!");
    }

    if (isOpened) {
        if (!anchorInputWindow) {
            anchorInputWindow = std::make_unique<AnchorInputWindow>(this, viewModel.get());
        }
        anchorInputWindow->exec();
    }


    viewModel->startTimer();
}

void IndoorPositioningSystemUI::onVideoOpened(int totalFrames, long long videoDuration)
{
    ui->pushButton_Play_Pause->setEnabled(true);
    ui->pushButton_Predict_Optical->setEnabled(true);
    ui->pushButton_Predict_Model->setEnabled(true);
    ui->pushButton_Export_Data->setEnabled(true);
    ui->pushButton_UWB_Localization->setEnabled(true);
    ui->pushButton_UWB_Data_Analysis->setEnabled(true);
    ui->horizontalSlider_Duration->setEnabled(true);

    ui->horizontalSlider_Duration->setRange(1, totalFrames);
    QTime totalTime = QTime(0, 0).addMSecs(static_cast<int>(videoDuration));
    ui->label_Total_Time->setText(totalTime.toString("HH:mm:ss"));
    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    uwbLocalizationWindow.reset();
    if (!uwbCoordinatesWindow) {
        uwbCoordinatesWindow = std::make_unique<CoordinatesWindow>(this, "UWB Coordinates", CoordinateWindowObjectType::Tag);
        connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateTagPosition, uwbCoordinatesWindow.get(), &CoordinatesWindow::updatePosition);

        // Get the current position of the main window
        QPoint currentWindowPos = this->pos();

        // Define the offset
        int offsetX = 1720;
        int offsetY = 50;

        // Calculate the new position for the uwbCoordinatesWindow
        QPoint newWindowPos = currentWindowPos + QPoint(offsetX, offsetY);

        // Move the uwbCoordinatesWindow to the new position
        uwbCoordinatesWindow->move(newWindowPos);
    }

    uwbCoordinatesWindow->show();
}

void IndoorPositioningSystemUI::loadPixelToRealModelParams() {
    viewModel->stopTimer();
    QString selectedFile = QFileDialog::getOpenFileName(this, "Select File", QDir::homePath(), "All Supported Files (*.model *.json);;XGBoost Model Files (*.model);;JSON Files (*.json)");

    if (!selectedFile.isEmpty()) {
        viewModel->loadPixelToRealModelParams(selectedFile);
    } else {
        QMessageBox::warning(this, "Warning", "No file was selected!");
    }
    viewModel->startTimer();
}


void IndoorPositioningSystemUI::on_actionLoad_model_params_triggered()
{
    loadPixelToRealModelParams();
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
    viewModel->startTimer();
}

void IndoorPositioningSystemUI::onModelNotLoaded(PredictionType type) {
    viewModel->stopTimer();
    QMessageBox::StandardButton reply;

    if (type == PredictionType::PredictionByPixelToReal) {
        reply = QMessageBox::question(this, "Prediction Model", "XGBoost prediction model is not loaded. Do you want to load one?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            loadPixelToRealModelParams();
        } else {
            viewModel->setPredictByPixelToReal(false);
        }
    } else  if (type == PredictionType::PredictionByOptical) {
        reply = QMessageBox::question(this, "Camera Calibration Parameters", "The camera calibration parameters are not loaded. Do you want to load one?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            loadIntrinsicCalibrationParams();
        } else {
            viewModel->setPredictionByOptical(false);
        }
    }
    viewModel->startTimer();
}

void IndoorPositioningSystemUI::on_actionLoad_human_detector_weights_triggered()
{
    loadHumanDetectorWeights();
}

void IndoorPositioningSystemUI::loadHumanDetectorWeights() {
    viewModel->stopTimer();
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", (QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));

    if (!directory.isEmpty()) {
        viewModel->loadHumanDetectorWeights(directory);
    } else {
        QMessageBox::warning(this, "Warning", "No directory was selected!");
    }
    viewModel->startTimer();
}

void IndoorPositioningSystemUI::onHumanDetectorNotInitialized() {
    viewModel->stopTimer();
    // QMessageBox::StandardButton reply;

    // reply = QMessageBox::question(this, "Human detector", "Wights and configuration file necessary for human detection are not loaded. Do you want to load them?",
    //                               QMessageBox::Yes|QMessageBox::No);
    // if (reply == QMessageBox::Yes) {
    //     loadHumanDetectorWeights();
    // } else {
    //     viewModel->setPredictByPixelToReal(false);
    //     viewModel->setPredictionByOptical(false);
    // }
    QMessageBox::warning(this, "Human detector", "Wights and configuration file necessary for human detection are not loaded. Please load them in 'file' menu.");
    viewModel->startTimer();
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
    if (!dataAnalysisWindow) {
        dataAnalysisWindow = std::make_unique<DataAnalysisWindow>(this, viewModel.get());
    }

    dataAnalysisWindow->show();
}

void IndoorPositioningSystemUI::on_pushButton_UWB_Localization_clicked()
{
    if (!uwbLocalizationWindow) {
        std::vector<AnchorPosition> anchorPositions = viewModel->getAnchorPositions();

         // = {QPointF(2.362, 0), QPointF(1.112, 2.08), QPointF(3.612, 2.08)}; // 2.362

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
    ui->horizontalSlider_Duration->setDisabled(true);
    ui->pushButton_Export_Data->setDisabled(true);
    ui->pushButton_UWB_Data_Analysis->setDisabled(true);
    ui->pushButton_UWB_Localization->setDisabled(true);
    ui->pushButton_Play_Pause->setDisabled(true);
    ui->pushButton_Predict_Optical->setDisabled(true);
    ui->pushButton_Predict_Model->setDisabled(true);

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
    viewModel->predict(PredictionType::PredictionByPixelToReal);
}


void IndoorPositioningSystemUI::on_pushButton_Predict_Optical_clicked()
{
   viewModel->predict(PredictionType::PredictionByOptical);
}

void IndoorPositioningSystemUI::onFinishedVideoProcessing() {
    QMessageBox::information(this, "Finish", "Video processing has completed successfully.");
}

void IndoorPositioningSystemUI::onShowWarning(const QString& header, const QString &message) {
    QMessageBox::warning(this, header, message);
}

void IndoorPositioningSystemUI::onChangePredictionButtonName(PredictionType type, bool isPredictionRequested) {
    if (isPredictionRequested)
    {
        if (type == PredictionType::PredictionByPixelToReal) {
            ui->pushButton_Predict_Model->setText("Stop prediction");
            ui->pushButton_Predict_Model->setToolTip("Stop pixel to real prediction");
            ui->pushButton_Pixel_to_Real_Show_Coordinates->setEnabled(true);
            if (!pixelToRealCoordinatesWindow) {
                pixelToRealCoordinatesWindow = std::make_unique<CoordinatesWindow>(this, "Pixel to Real Coordinates", CoordinateWindowObjectType::Person);
                connect(viewModel.get(), &IndoorPositioningSystemViewModel::updatePixelToRealPosition, pixelToRealCoordinatesWindow.get(), &CoordinatesWindow::updatePosition);
                // Get the current position of the main window
                QPoint currentWindowPos = this->pos();

                // Define the offset
                int offsetX = 1720;
                int offsetY = 800;

                // Calculate the new position for the uwbCoordinatesWindow
                QPoint newWindowPos = currentWindowPos + QPoint(offsetX, offsetY);

                // Move the uwbCoordinatesWindow to the new position
                pixelToRealCoordinatesWindow->move(newWindowPos);
            }
            pixelToRealCoordinatesWindow->show();
        } else if (type == PredictionType::PredictionByOptical) {
            ui->pushButton_Predict_Optical->setText("Stop prediction");
            ui->pushButton_Predict_Optical->setToolTip("Stop optical method");
            ui->pushButton_Optical_Show_Coordinates->setEnabled(true);
            if (!opticalCoordinatesWindow) {
                opticalCoordinatesWindow = std::make_unique<CoordinatesWindow>(this, "Optical Coordinates", CoordinateWindowObjectType::Person);
                connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateOpticalPosition, opticalCoordinatesWindow.get(), &CoordinatesWindow::updatePosition);
                // Get the current position of the main window
                QPoint currentWindowPos = this->pos();

                // Define the offset
                int offsetX = 1720;
                int offsetY = 400;

                // Calculate the new position for the uwbCoordinatesWindow
                QPoint newWindowPos = currentWindowPos + QPoint(offsetX, offsetY);

                // Move the uwbCoordinatesWindow to the new position
                opticalCoordinatesWindow->move(newWindowPos);
            }
            opticalCoordinatesWindow->show();
        }
    } else {
        if (type == PredictionType::PredictionByPixelToReal) {
            ui->pushButton_Predict_Model->setText("Start Pixel-to-real");
            ui->pushButton_Predict_Model->setToolTip("Start pixel to real model for coordinates prediction");
            ui->pushButton_Pixel_to_Real_Show_Coordinates->setDisabled(true);
            if (pixelToRealCoordinatesWindow && !pixelToRealCoordinatesWindow->isHidden()) {
                pixelToRealCoordinatesWindow->hide();
            }
        } else if (type == PredictionType::PredictionByOptical) {
            ui->pushButton_Predict_Optical->setText("Start Optical method");
            ui->pushButton_Predict_Optical->setToolTip("Start optical method for coordinates estimation");
            ui->pushButton_Optical_Show_Coordinates->setDisabled(true);
            if (opticalCoordinatesWindow && !opticalCoordinatesWindow->isHidden()) {
                opticalCoordinatesWindow->hide();
            }
        }
    }
}


void IndoorPositioningSystemUI::on_pushButton_UWB_Show_Coordinates_clicked()
{
    if (!uwbCoordinatesWindow) {
        uwbCoordinatesWindow = std::make_unique<CoordinatesWindow>(this, "UWB Coordinates", CoordinateWindowObjectType::Tag);
        connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateTagPosition, uwbCoordinatesWindow.get(), &CoordinatesWindow::updatePosition);
    }

    uwbCoordinatesWindow->show();

}


void IndoorPositioningSystemUI::on_pushButton_Optical_Show_Coordinates_clicked()
{
    if (!opticalCoordinatesWindow) {
        opticalCoordinatesWindow = std::make_unique<CoordinatesWindow>(this, "Optical Coordinates", CoordinateWindowObjectType::Person);
        connect(viewModel.get(), &IndoorPositioningSystemViewModel::updateOpticalPosition, opticalCoordinatesWindow.get(), &CoordinatesWindow::updatePosition);
    }

    opticalCoordinatesWindow->show();
}


void IndoorPositioningSystemUI::on_pushButton_Pixel_to_Real_Show_Coordinates_clicked()
{
    if (!pixelToRealCoordinatesWindow) {
        pixelToRealCoordinatesWindow = std::make_unique<CoordinatesWindow>(this, "Pixel to Real Coordinates", CoordinateWindowObjectType::Person);
        connect(viewModel.get(), &IndoorPositioningSystemViewModel::updatePixelToRealPosition, pixelToRealCoordinatesWindow.get(), &CoordinatesWindow::updatePosition);
    }

    pixelToRealCoordinatesWindow->show();

}

void IndoorPositioningSystemUI::onDistCoeffsLoaded() {
    QMessageBox::information(this, "Distortion Coefficients", "Distortion coefficients are successfully applied to the image!");
}

/********************************************************************************
** Form generated from reading UI file 'indoorpositioningsystemui.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDOORPOSITIONINGSYSTEMUI_H
#define UI_INDOORPOSITIONINGSYSTEMUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IndoorPositioningSystemUI
{
public:
    QAction *actionOpen_Video;
    QAction *actionOpen_UWB_data;
    QAction *actionLoad_model_params;
    QAction *actionLoad_intrinsic_calibration_params;
    QAction *actionLoad_human_detector_weights;
    QWidget *centralwidget;
    QPushButton *pushButton_Play_Pause;
    QSlider *horizontalSlider_Duration;
    QLabel *label_Current_Time;
    QLabel *label_Total_Time;
    QLabel *label_Video;
    QLabel *label_Video_Frame_ID;
    QLabel *label_Video_Frame_Timestamp;
    QLabel *label_Tag_ID_1;
    QLabel *label_Tag_timestamp_1;
    QLabel *label_Anchor1_ID_1;
    QLabel *label_Anchor2_Distance_1;
    QLabel *label_Video_Frame_ID_value;
    QLabel *label_Video_Frame_Timestamp_value;
    QLabel *label_Tag_ID_value_1;
    QLabel *label_Tag_timestamp_value_1;
    QLabel *label_Anchor1_Distance_value_1;
    QLabel *label_Anchor2_Distance_value_1;
    QLabel *label_Tag_ID_value_2;
    QLabel *label_Anchor2_Distance_2;
    QLabel *label_Tag_ID_2;
    QLabel *label_Anchor1_Distance_value_2;
    QLabel *label_Anchor2_Distance_value_2;
    QLabel *label_Anchor1_Distance_2;
    QLabel *label_Tag_timestamp_2;
    QLabel *label_Tag_timestamp_value_2;
    QLabel *label_Tag_ID_value_3;
    QLabel *label_Tag_ID_3;
    QLabel *label_Anchor1_ID_3;
    QLabel *label_Tag_timestamp_3;
    QLabel *label_Tag_timestamp_value_3;
    QPushButton *pushButton_UWB_Data_Analysis;
    QLabel *label_Video_Stream;
    QFrame *line;
    QLabel *label_Video_Stream_2;
    QPushButton *pushButton_UWB_Localization;
    QPushButton *pushButton_Export_Data;
    QPushButton *pushButton_Predict_Model;
    QPushButton *pushButton_Predict_Optical;
    QLabel *label_Tag_measurement_time_1;
    QLabel *label_Tag_measurement_time_value_1;
    QLabel *label_Tag_measurement_time_2;
    QLabel *label_Tag_measurement_time_value_2;
    QLabel *label_Tag_measurement_time_3;
    QLabel *label_Tag_measurement_time_value_3;
    QLabel *label_Predict_coordinates;
    QPushButton *pushButton_UWB_Show_Coordinates;
    QLabel *label_Predict_coordinates_2;
    QPushButton *pushButton_Optical_Show_Coordinates;
    QPushButton *pushButton_Pixel_to_Real_Show_Coordinates;
    QLabel *label_Anchor1_Distance_1;
    QLabel *label_Anchor1_ID_value_1;
    QLabel *label_Anchor2_ID_1;
    QLabel *label_Anchor2_ID_value_1;
    QFrame *line_2;
    QFrame *line_3;
    QLabel *label_Anchor2_ID_2;
    QLabel *label_Anchor2_ID_value_2;
    QLabel *label_Anchor1_ID_2;
    QLabel *label_Anchor1_ID_value_2;
    QFrame *line_4;
    QLabel *label_Anchor2_ID_3;
    QLabel *label_Anchor2_ID_value_3;
    QLabel *label_Anchor1_ID_value_3;
    QLabel *label_Anchor1_Distance_value_3;
    QLabel *label_Anchor2_Distance_3;
    QLabel *label_Anchor1_Distance_3;
    QLabel *label_Anchor2_Distance_value_3;
    QFrame *line_5;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *IndoorPositioningSystemUI)
    {
        if (IndoorPositioningSystemUI->objectName().isEmpty())
            IndoorPositioningSystemUI->setObjectName("IndoorPositioningSystemUI");
        IndoorPositioningSystemUI->resize(1708, 996);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(IndoorPositioningSystemUI->sizePolicy().hasHeightForWidth());
        IndoorPositioningSystemUI->setSizePolicy(sizePolicy);
        IndoorPositioningSystemUI->setMaximumSize(QSize(2136, 16777215));
        actionOpen_Video = new QAction(IndoorPositioningSystemUI);
        actionOpen_Video->setObjectName("actionOpen_Video");
        actionOpen_UWB_data = new QAction(IndoorPositioningSystemUI);
        actionOpen_UWB_data->setObjectName("actionOpen_UWB_data");
        actionLoad_model_params = new QAction(IndoorPositioningSystemUI);
        actionLoad_model_params->setObjectName("actionLoad_model_params");
        actionLoad_intrinsic_calibration_params = new QAction(IndoorPositioningSystemUI);
        actionLoad_intrinsic_calibration_params->setObjectName("actionLoad_intrinsic_calibration_params");
        actionLoad_human_detector_weights = new QAction(IndoorPositioningSystemUI);
        actionLoad_human_detector_weights->setObjectName("actionLoad_human_detector_weights");
        centralwidget = new QWidget(IndoorPositioningSystemUI);
        centralwidget->setObjectName("centralwidget");
        pushButton_Play_Pause = new QPushButton(centralwidget);
        pushButton_Play_Pause->setObjectName("pushButton_Play_Pause");
        pushButton_Play_Pause->setGeometry(QRect(10, 880, 41, 40));
        horizontalSlider_Duration = new QSlider(centralwidget);
        horizontalSlider_Duration->setObjectName("horizontalSlider_Duration");
        horizontalSlider_Duration->setGeometry(QRect(100, 840, 1141, 19));
        horizontalSlider_Duration->setMouseTracking(false);
        horizontalSlider_Duration->setOrientation(Qt::Horizontal);
        label_Current_Time = new QLabel(centralwidget);
        label_Current_Time->setObjectName("label_Current_Time");
        label_Current_Time->setGeometry(QRect(30, 840, 67, 16));
        label_Total_Time = new QLabel(centralwidget);
        label_Total_Time->setObjectName("label_Total_Time");
        label_Total_Time->setGeometry(QRect(1250, 840, 67, 16));
        label_Video = new QLabel(centralwidget);
        label_Video->setObjectName("label_Video");
        label_Video->setGeometry(QRect(30, 90, 1281, 721));
        label_Video_Frame_ID = new QLabel(centralwidget);
        label_Video_Frame_ID->setObjectName("label_Video_Frame_ID");
        label_Video_Frame_ID->setGeometry(QRect(170, 30, 71, 20));
        label_Video_Frame_Timestamp = new QLabel(centralwidget);
        label_Video_Frame_Timestamp->setObjectName("label_Video_Frame_Timestamp");
        label_Video_Frame_Timestamp->setGeometry(QRect(380, 32, 141, 17));
        label_Tag_ID_1 = new QLabel(centralwidget);
        label_Tag_ID_1->setObjectName("label_Tag_ID_1");
        label_Tag_ID_1->setGeometry(QRect(1340, 80, 51, 17));
        label_Tag_timestamp_1 = new QLabel(centralwidget);
        label_Tag_timestamp_1->setObjectName("label_Tag_timestamp_1");
        label_Tag_timestamp_1->setGeometry(QRect(1450, 79, 81, 20));
        label_Anchor1_ID_1 = new QLabel(centralwidget);
        label_Anchor1_ID_1->setObjectName("label_Anchor1_ID_1");
        label_Anchor1_ID_1->setGeometry(QRect(1340, 110, 81, 17));
        label_Anchor2_Distance_1 = new QLabel(centralwidget);
        label_Anchor2_Distance_1->setObjectName("label_Anchor2_Distance_1");
        label_Anchor2_Distance_1->setGeometry(QRect(1490, 130, 71, 17));
        label_Video_Frame_ID_value = new QLabel(centralwidget);
        label_Video_Frame_ID_value->setObjectName("label_Video_Frame_ID_value");
        label_Video_Frame_ID_value->setGeometry(QRect(240, 30, 131, 20));
        label_Video_Frame_Timestamp_value = new QLabel(centralwidget);
        label_Video_Frame_Timestamp_value->setObjectName("label_Video_Frame_Timestamp_value");
        label_Video_Frame_Timestamp_value->setGeometry(QRect(510, 31, 151, 20));
        label_Tag_ID_value_1 = new QLabel(centralwidget);
        label_Tag_ID_value_1->setObjectName("label_Tag_ID_value_1");
        label_Tag_ID_value_1->setGeometry(QRect(1390, 81, 51, 17));
        label_Tag_timestamp_value_1 = new QLabel(centralwidget);
        label_Tag_timestamp_value_1->setObjectName("label_Tag_timestamp_value_1");
        label_Tag_timestamp_value_1->setGeometry(QRect(1536, 80, 161, 20));
        label_Anchor1_Distance_value_1 = new QLabel(centralwidget);
        label_Anchor1_Distance_value_1->setObjectName("label_Anchor1_Distance_value_1");
        label_Anchor1_Distance_value_1->setGeometry(QRect(1560, 110, 111, 17));
        label_Anchor2_Distance_value_1 = new QLabel(centralwidget);
        label_Anchor2_Distance_value_1->setObjectName("label_Anchor2_Distance_value_1");
        label_Anchor2_Distance_value_1->setGeometry(QRect(1560, 130, 131, 20));
        label_Tag_ID_value_2 = new QLabel(centralwidget);
        label_Tag_ID_value_2->setObjectName("label_Tag_ID_value_2");
        label_Tag_ID_value_2->setGeometry(QRect(1390, 213, 51, 17));
        label_Anchor2_Distance_2 = new QLabel(centralwidget);
        label_Anchor2_Distance_2->setObjectName("label_Anchor2_Distance_2");
        label_Anchor2_Distance_2->setGeometry(QRect(1490, 260, 61, 17));
        label_Tag_ID_2 = new QLabel(centralwidget);
        label_Tag_ID_2->setObjectName("label_Tag_ID_2");
        label_Tag_ID_2->setGeometry(QRect(1340, 210, 51, 20));
        label_Anchor1_Distance_value_2 = new QLabel(centralwidget);
        label_Anchor1_Distance_value_2->setObjectName("label_Anchor1_Distance_value_2");
        label_Anchor1_Distance_value_2->setGeometry(QRect(1560, 240, 101, 17));
        label_Anchor2_Distance_value_2 = new QLabel(centralwidget);
        label_Anchor2_Distance_value_2->setObjectName("label_Anchor2_Distance_value_2");
        label_Anchor2_Distance_value_2->setGeometry(QRect(1560, 260, 121, 17));
        label_Anchor1_Distance_2 = new QLabel(centralwidget);
        label_Anchor1_Distance_2->setObjectName("label_Anchor1_Distance_2");
        label_Anchor1_Distance_2->setGeometry(QRect(1490, 240, 61, 17));
        label_Tag_timestamp_2 = new QLabel(centralwidget);
        label_Tag_timestamp_2->setObjectName("label_Tag_timestamp_2");
        label_Tag_timestamp_2->setGeometry(QRect(1450, 210, 81, 20));
        label_Tag_timestamp_value_2 = new QLabel(centralwidget);
        label_Tag_timestamp_value_2->setObjectName("label_Tag_timestamp_value_2");
        label_Tag_timestamp_value_2->setGeometry(QRect(1537, 212, 161, 17));
        label_Tag_ID_value_3 = new QLabel(centralwidget);
        label_Tag_ID_value_3->setObjectName("label_Tag_ID_value_3");
        label_Tag_ID_value_3->setGeometry(QRect(1393, 342, 71, 17));
        label_Tag_ID_3 = new QLabel(centralwidget);
        label_Tag_ID_3->setObjectName("label_Tag_ID_3");
        label_Tag_ID_3->setGeometry(QRect(1340, 340, 51, 20));
        label_Anchor1_ID_3 = new QLabel(centralwidget);
        label_Anchor1_ID_3->setObjectName("label_Anchor1_ID_3");
        label_Anchor1_ID_3->setGeometry(QRect(1340, 380, 81, 17));
        label_Tag_timestamp_3 = new QLabel(centralwidget);
        label_Tag_timestamp_3->setObjectName("label_Tag_timestamp_3");
        label_Tag_timestamp_3->setGeometry(QRect(1450, 339, 81, 20));
        label_Tag_timestamp_value_3 = new QLabel(centralwidget);
        label_Tag_timestamp_value_3->setObjectName("label_Tag_timestamp_value_3");
        label_Tag_timestamp_value_3->setGeometry(QRect(1538, 342, 151, 17));
        pushButton_UWB_Data_Analysis = new QPushButton(centralwidget);
        pushButton_UWB_Data_Analysis->setObjectName("pushButton_UWB_Data_Analysis");
        pushButton_UWB_Data_Analysis->setGeometry(QRect(1560, 490, 141, 25));
        label_Video_Stream = new QLabel(centralwidget);
        label_Video_Stream->setObjectName("label_Video_Stream");
        label_Video_Stream->setGeometry(QRect(40, 31, 121, 17));
        label_Video_Stream->setStyleSheet(QString::fromUtf8("font: 700 13pt \"Ubuntu\";"));
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(1320, 20, 21, 851));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label_Video_Stream_2 = new QLabel(centralwidget);
        label_Video_Stream_2->setObjectName("label_Video_Stream_2");
        label_Video_Stream_2->setGeometry(QRect(1350, 30, 151, 17));
        label_Video_Stream_2->setStyleSheet(QString::fromUtf8("font: 700 13pt \"Ubuntu\";"));
        pushButton_UWB_Localization = new QPushButton(centralwidget);
        pushButton_UWB_Localization->setObjectName("pushButton_UWB_Localization");
        pushButton_UWB_Localization->setGeometry(QRect(1340, 530, 181, 25));
        pushButton_Export_Data = new QPushButton(centralwidget);
        pushButton_Export_Data->setObjectName("pushButton_Export_Data");
        pushButton_Export_Data->setGeometry(QRect(1460, 900, 241, 25));
        pushButton_Predict_Model = new QPushButton(centralwidget);
        pushButton_Predict_Model->setObjectName("pushButton_Predict_Model");
        pushButton_Predict_Model->setGeometry(QRect(1030, 920, 151, 25));
        pushButton_Predict_Optical = new QPushButton(centralwidget);
        pushButton_Predict_Optical->setObjectName("pushButton_Predict_Optical");
        pushButton_Predict_Optical->setGeometry(QRect(1030, 879, 151, 25));
        label_Tag_measurement_time_1 = new QLabel(centralwidget);
        label_Tag_measurement_time_1->setObjectName("label_Tag_measurement_time_1");
        label_Tag_measurement_time_1->setGeometry(QRect(1340, 160, 170, 20));
        label_Tag_measurement_time_value_1 = new QLabel(centralwidget);
        label_Tag_measurement_time_value_1->setObjectName("label_Tag_measurement_time_value_1");
        label_Tag_measurement_time_value_1->setGeometry(QRect(1512, 163, 141, 17));
        label_Tag_measurement_time_2 = new QLabel(centralwidget);
        label_Tag_measurement_time_2->setObjectName("label_Tag_measurement_time_2");
        label_Tag_measurement_time_2->setGeometry(QRect(1340, 290, 170, 20));
        label_Tag_measurement_time_value_2 = new QLabel(centralwidget);
        label_Tag_measurement_time_value_2->setObjectName("label_Tag_measurement_time_value_2");
        label_Tag_measurement_time_value_2->setGeometry(QRect(1514, 292, 141, 17));
        label_Tag_measurement_time_3 = new QLabel(centralwidget);
        label_Tag_measurement_time_3->setObjectName("label_Tag_measurement_time_3");
        label_Tag_measurement_time_3->setGeometry(QRect(1340, 440, 170, 20));
        label_Tag_measurement_time_value_3 = new QLabel(centralwidget);
        label_Tag_measurement_time_value_3->setObjectName("label_Tag_measurement_time_value_3");
        label_Tag_measurement_time_value_3->setGeometry(QRect(1514, 442, 141, 17));
        label_Predict_coordinates = new QLabel(centralwidget);
        label_Predict_coordinates->setObjectName("label_Predict_coordinates");
        label_Predict_coordinates->setGeometry(QRect(740, 880, 281, 20));
        QFont font;
        font.setPointSize(12);
        label_Predict_coordinates->setFont(font);
        pushButton_UWB_Show_Coordinates = new QPushButton(centralwidget);
        pushButton_UWB_Show_Coordinates->setObjectName("pushButton_UWB_Show_Coordinates");
        pushButton_UWB_Show_Coordinates->setGeometry(QRect(1530, 530, 171, 25));
        label_Predict_coordinates_2 = new QLabel(centralwidget);
        label_Predict_coordinates_2->setObjectName("label_Predict_coordinates_2");
        label_Predict_coordinates_2->setGeometry(QRect(700, 920, 321, 20));
        label_Predict_coordinates_2->setFont(font);
        pushButton_Optical_Show_Coordinates = new QPushButton(centralwidget);
        pushButton_Optical_Show_Coordinates->setObjectName("pushButton_Optical_Show_Coordinates");
        pushButton_Optical_Show_Coordinates->setGeometry(QRect(1190, 879, 131, 25));
        pushButton_Pixel_to_Real_Show_Coordinates = new QPushButton(centralwidget);
        pushButton_Pixel_to_Real_Show_Coordinates->setObjectName("pushButton_Pixel_to_Real_Show_Coordinates");
        pushButton_Pixel_to_Real_Show_Coordinates->setGeometry(QRect(1190, 920, 131, 25));
        label_Anchor1_Distance_1 = new QLabel(centralwidget);
        label_Anchor1_Distance_1->setObjectName("label_Anchor1_Distance_1");
        label_Anchor1_Distance_1->setGeometry(QRect(1490, 110, 61, 17));
        label_Anchor1_ID_value_1 = new QLabel(centralwidget);
        label_Anchor1_ID_value_1->setObjectName("label_Anchor1_ID_value_1");
        label_Anchor1_ID_value_1->setGeometry(QRect(1420, 110, 51, 17));
        label_Anchor2_ID_1 = new QLabel(centralwidget);
        label_Anchor2_ID_1->setObjectName("label_Anchor2_ID_1");
        label_Anchor2_ID_1->setGeometry(QRect(1340, 132, 81, 17));
        label_Anchor2_ID_value_1 = new QLabel(centralwidget);
        label_Anchor2_ID_value_1->setObjectName("label_Anchor2_ID_value_1");
        label_Anchor2_ID_value_1->setGeometry(QRect(1420, 131, 51, 17));
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(1340, 190, 351, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralwidget);
        line_3->setObjectName("line_3");
        line_3->setGeometry(QRect(1510, 30, 181, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        label_Anchor2_ID_2 = new QLabel(centralwidget);
        label_Anchor2_ID_2->setObjectName("label_Anchor2_ID_2");
        label_Anchor2_ID_2->setGeometry(QRect(1340, 262, 81, 17));
        label_Anchor2_ID_value_2 = new QLabel(centralwidget);
        label_Anchor2_ID_value_2->setObjectName("label_Anchor2_ID_value_2");
        label_Anchor2_ID_value_2->setGeometry(QRect(1420, 261, 51, 17));
        label_Anchor1_ID_2 = new QLabel(centralwidget);
        label_Anchor1_ID_2->setObjectName("label_Anchor1_ID_2");
        label_Anchor1_ID_2->setGeometry(QRect(1340, 240, 81, 17));
        label_Anchor1_ID_value_2 = new QLabel(centralwidget);
        label_Anchor1_ID_value_2->setObjectName("label_Anchor1_ID_value_2");
        label_Anchor1_ID_value_2->setGeometry(QRect(1420, 239, 51, 17));
        line_4 = new QFrame(centralwidget);
        line_4->setObjectName("line_4");
        line_4->setGeometry(QRect(1340, 320, 351, 16));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        label_Anchor2_ID_3 = new QLabel(centralwidget);
        label_Anchor2_ID_3->setObjectName("label_Anchor2_ID_3");
        label_Anchor2_ID_3->setGeometry(QRect(1340, 400, 81, 17));
        label_Anchor2_ID_value_3 = new QLabel(centralwidget);
        label_Anchor2_ID_value_3->setObjectName("label_Anchor2_ID_value_3");
        label_Anchor2_ID_value_3->setGeometry(QRect(1420, 400, 51, 17));
        label_Anchor1_ID_value_3 = new QLabel(centralwidget);
        label_Anchor1_ID_value_3->setObjectName("label_Anchor1_ID_value_3");
        label_Anchor1_ID_value_3->setGeometry(QRect(1420, 379, 51, 17));
        label_Anchor1_Distance_value_3 = new QLabel(centralwidget);
        label_Anchor1_Distance_value_3->setObjectName("label_Anchor1_Distance_value_3");
        label_Anchor1_Distance_value_3->setGeometry(QRect(1560, 381, 101, 17));
        label_Anchor2_Distance_3 = new QLabel(centralwidget);
        label_Anchor2_Distance_3->setObjectName("label_Anchor2_Distance_3");
        label_Anchor2_Distance_3->setGeometry(QRect(1490, 400, 61, 17));
        label_Anchor1_Distance_3 = new QLabel(centralwidget);
        label_Anchor1_Distance_3->setObjectName("label_Anchor1_Distance_3");
        label_Anchor1_Distance_3->setGeometry(QRect(1490, 380, 61, 17));
        label_Anchor2_Distance_value_3 = new QLabel(centralwidget);
        label_Anchor2_Distance_value_3->setObjectName("label_Anchor2_Distance_value_3");
        label_Anchor2_Distance_value_3->setGeometry(QRect(1560, 400, 121, 17));
        line_5 = new QFrame(centralwidget);
        line_5->setObjectName("line_5");
        line_5->setGeometry(QRect(1340, 470, 351, 16));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        IndoorPositioningSystemUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(IndoorPositioningSystemUI);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1708, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        IndoorPositioningSystemUI->setMenuBar(menubar);
        statusbar = new QStatusBar(IndoorPositioningSystemUI);
        statusbar->setObjectName("statusbar");
        IndoorPositioningSystemUI->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Video);
        menuFile->addAction(actionLoad_human_detector_weights);
        menuFile->addAction(actionLoad_model_params);
        menuFile->addAction(actionLoad_intrinsic_calibration_params);

        retranslateUi(IndoorPositioningSystemUI);

        QMetaObject::connectSlotsByName(IndoorPositioningSystemUI);
    } // setupUi

    void retranslateUi(QMainWindow *IndoorPositioningSystemUI)
    {
        IndoorPositioningSystemUI->setWindowTitle(QCoreApplication::translate("IndoorPositioningSystemUI", "IndoorPositioningSystem", nullptr));
        actionOpen_Video->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Open video", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen_Video->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "The folder should include video data as well as UWB data", nullptr));
#endif // QT_CONFIG(tooltip)
        actionOpen_UWB_data->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Open UWB data", nullptr));
        actionLoad_model_params->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Load pixel to real model params", nullptr));
        actionLoad_intrinsic_calibration_params->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Load camera intrinsic params", nullptr));
        actionLoad_human_detector_weights->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Load human detection model weights", nullptr));
        pushButton_Play_Pause->setText(QString());
        label_Current_Time->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "00:00:00", nullptr));
        label_Total_Time->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "00:00:00", nullptr));
        label_Video->setText(QString());
        label_Video_Frame_ID->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Frame ID:", nullptr));
        label_Video_Frame_Timestamp->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Frame Timestamp:", nullptr));
        label_Tag_ID_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Tag ID:", nullptr));
        label_Tag_timestamp_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Timestamp:", nullptr));
        label_Anchor1_ID_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Anchor ID:", nullptr));
        label_Anchor2_Distance_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Distance:", nullptr));
        label_Video_Frame_ID_value->setText(QString());
        label_Video_Frame_Timestamp_value->setText(QString());
        label_Tag_ID_value_1->setText(QString());
        label_Tag_timestamp_value_1->setText(QString());
        label_Anchor1_Distance_value_1->setText(QString());
        label_Anchor2_Distance_value_1->setText(QString());
        label_Tag_ID_value_2->setText(QString());
        label_Anchor2_Distance_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Distance:", nullptr));
        label_Tag_ID_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Tag ID:", nullptr));
        label_Anchor1_Distance_value_2->setText(QString());
        label_Anchor2_Distance_value_2->setText(QString());
        label_Anchor1_Distance_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Distance:", nullptr));
        label_Tag_timestamp_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Timestamp:", nullptr));
        label_Tag_timestamp_value_2->setText(QString());
        label_Tag_ID_value_3->setText(QString());
        label_Tag_ID_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Tag ID:", nullptr));
        label_Anchor1_ID_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Anchor ID:", nullptr));
        label_Tag_timestamp_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Timestamp:", nullptr));
        label_Tag_timestamp_value_3->setText(QString());
        pushButton_UWB_Data_Analysis->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Analyse UWB Data", nullptr));
#if QT_CONFIG(tooltip)
        label_Video_Stream->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Information about the video frame", nullptr));
#endif // QT_CONFIG(tooltip)
        label_Video_Stream->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Video Stream:", nullptr));
#if QT_CONFIG(tooltip)
        label_Video_Stream_2->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Information about the UWB data", nullptr));
#endif // QT_CONFIG(tooltip)
        label_Video_Stream_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "UWB Data Stream:", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_UWB_Localization->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Visualize localization", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_UWB_Localization->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "UWB Localization Schema", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_Export_Data->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Export data for Pixel to Real model. This includes UWB coordinates synchronized with video frames", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_Export_Data->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Export Data Frame by Frame", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_Predict_Model->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Start pixel to real model for coordinates prediction", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_Predict_Model->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Start Pixel-to-real", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_Predict_Optical->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Start optical method for coordinates estimation", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_Predict_Optical->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Start Optical method", nullptr));
        label_Tag_measurement_time_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Measurement time (ms):", nullptr));
        label_Tag_measurement_time_value_1->setText(QString());
        label_Tag_measurement_time_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Measurement time (ms):", nullptr));
        label_Tag_measurement_time_value_2->setText(QString());
        label_Tag_measurement_time_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Measurement time (ms):", nullptr));
        label_Tag_measurement_time_value_3->setText(QString());
        label_Predict_coordinates->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Predict coordinates by Optical method:", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_UWB_Show_Coordinates->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Visualize localization", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_UWB_Show_Coordinates->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Show UWB Coordinates", nullptr));
        label_Predict_coordinates_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Predict coordinates by Pixel-to-Real method:", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_Optical_Show_Coordinates->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Visualize localization", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_Optical_Show_Coordinates->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Show Coordinates", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_Pixel_to_Real_Show_Coordinates->setToolTip(QCoreApplication::translate("IndoorPositioningSystemUI", "Visualize localization", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_Pixel_to_Real_Show_Coordinates->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Show Coordinates", nullptr));
        label_Anchor1_Distance_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Distance: ", nullptr));
        label_Anchor1_ID_value_1->setText(QString());
        label_Anchor2_ID_1->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Anchor ID:", nullptr));
        label_Anchor2_ID_value_1->setText(QString());
        label_Anchor2_ID_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Anchor ID:", nullptr));
        label_Anchor2_ID_value_2->setText(QString());
        label_Anchor1_ID_2->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Anchor ID:", nullptr));
        label_Anchor1_ID_value_2->setText(QString());
        label_Anchor2_ID_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Anchor ID:", nullptr));
        label_Anchor2_ID_value_3->setText(QString());
        label_Anchor1_ID_value_3->setText(QString());
        label_Anchor1_Distance_value_3->setText(QString());
        label_Anchor2_Distance_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Distance:", nullptr));
        label_Anchor1_Distance_3->setText(QCoreApplication::translate("IndoorPositioningSystemUI", "Distance:", nullptr));
        label_Anchor2_Distance_value_3->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("IndoorPositioningSystemUI", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IndoorPositioningSystemUI: public Ui_IndoorPositioningSystemUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDOORPOSITIONINGSYSTEMUI_H

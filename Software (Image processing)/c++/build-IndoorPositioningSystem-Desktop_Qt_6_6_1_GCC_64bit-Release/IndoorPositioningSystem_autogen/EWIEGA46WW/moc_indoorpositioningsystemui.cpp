/****************************************************************************
** Meta object code from reading C++ file 'indoorpositioningsystemui.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../IndoorPositioningSystem/indoorpositioningsystemui.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtNetwork/QSslPreSharedKeyAuthenticator>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'indoorpositioningsystemui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS = QtMocHelpers::stringData(
    "IndoorPositioningSystemUI",
    "tagPositionUpdated",
    "",
    "position",
    "tagID",
    "on_horizontalSlider_Duration_valueChanged",
    "on_pushButton_Play_Pause_clicked",
    "on_actionOpen_Video_triggered",
    "on_horizontalSlider_Duration_sliderReleased",
    "on_pushButton_UWB_Data_Analysis_clicked",
    "on_pushButton_UWB_Localization_clicked",
    "on_pushButton_Export_Data_clicked",
    "onAcceptFrameByFrameExport",
    "on_actionLoad_model_params_triggered",
    "on_pushButton_Predict_Optical_clicked",
    "on_pushButton_Predict_Model_clicked",
    "on_actionLoad_intrinsic_calibration_params_triggered",
    "onDataUpdated",
    "image",
    "frameID",
    "timestamp",
    "onUWBDataUpdated",
    "UWBData",
    "tag",
    "onFinishedVideoProcessing",
    "onDurationUpdated",
    "currentTimeInMSeconds",
    "onVideoOpened",
    "totalFrames",
    "videoDuration",
    "onFileLoaded",
    "success",
    "message",
    "onShowExportWarning",
    "header",
    "ExportType",
    "type",
    "onShowExportProcessDialog",
    "onUpdateExportProgress",
    "proportion",
    "onExportFinished",
    "onModelNotLoaded",
    "PredictionType",
    "onPositionUpdated",
    "currentTime",
    "onShowWarning",
    "onChangePredictionButtonName",
    "isPredictionRequested",
    "onHumanDetectorNotInitialized",
    "on_actionLoad_human_detector_weights_triggered",
    "on_pushButton_UWB_Show_Coordinates_clicked",
    "on_pushButton_Optical_Show_Coordinates_clicked",
    "on_pushButton_Pixel_to_Real_Show_Coordinates_clicked",
    "onDistCoeffsLoaded"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS_t {
    uint offsetsAndSizes[108];
    char stringdata0[26];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[6];
    char stringdata5[42];
    char stringdata6[33];
    char stringdata7[30];
    char stringdata8[44];
    char stringdata9[40];
    char stringdata10[39];
    char stringdata11[34];
    char stringdata12[27];
    char stringdata13[37];
    char stringdata14[38];
    char stringdata15[36];
    char stringdata16[53];
    char stringdata17[14];
    char stringdata18[6];
    char stringdata19[8];
    char stringdata20[10];
    char stringdata21[17];
    char stringdata22[8];
    char stringdata23[4];
    char stringdata24[26];
    char stringdata25[18];
    char stringdata26[22];
    char stringdata27[14];
    char stringdata28[12];
    char stringdata29[14];
    char stringdata30[13];
    char stringdata31[8];
    char stringdata32[8];
    char stringdata33[20];
    char stringdata34[7];
    char stringdata35[11];
    char stringdata36[5];
    char stringdata37[26];
    char stringdata38[23];
    char stringdata39[11];
    char stringdata40[17];
    char stringdata41[17];
    char stringdata42[15];
    char stringdata43[18];
    char stringdata44[12];
    char stringdata45[14];
    char stringdata46[29];
    char stringdata47[22];
    char stringdata48[30];
    char stringdata49[47];
    char stringdata50[43];
    char stringdata51[47];
    char stringdata52[53];
    char stringdata53[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS_t qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS = {
    {
        QT_MOC_LITERAL(0, 25),  // "IndoorPositioningSystemUI"
        QT_MOC_LITERAL(26, 18),  // "tagPositionUpdated"
        QT_MOC_LITERAL(45, 0),  // ""
        QT_MOC_LITERAL(46, 8),  // "position"
        QT_MOC_LITERAL(55, 5),  // "tagID"
        QT_MOC_LITERAL(61, 41),  // "on_horizontalSlider_Duration_..."
        QT_MOC_LITERAL(103, 32),  // "on_pushButton_Play_Pause_clicked"
        QT_MOC_LITERAL(136, 29),  // "on_actionOpen_Video_triggered"
        QT_MOC_LITERAL(166, 43),  // "on_horizontalSlider_Duration_..."
        QT_MOC_LITERAL(210, 39),  // "on_pushButton_UWB_Data_Analys..."
        QT_MOC_LITERAL(250, 38),  // "on_pushButton_UWB_Localizatio..."
        QT_MOC_LITERAL(289, 33),  // "on_pushButton_Export_Data_cli..."
        QT_MOC_LITERAL(323, 26),  // "onAcceptFrameByFrameExport"
        QT_MOC_LITERAL(350, 36),  // "on_actionLoad_model_params_tr..."
        QT_MOC_LITERAL(387, 37),  // "on_pushButton_Predict_Optical..."
        QT_MOC_LITERAL(425, 35),  // "on_pushButton_Predict_Model_c..."
        QT_MOC_LITERAL(461, 52),  // "on_actionLoad_intrinsic_calib..."
        QT_MOC_LITERAL(514, 13),  // "onDataUpdated"
        QT_MOC_LITERAL(528, 5),  // "image"
        QT_MOC_LITERAL(534, 7),  // "frameID"
        QT_MOC_LITERAL(542, 9),  // "timestamp"
        QT_MOC_LITERAL(552, 16),  // "onUWBDataUpdated"
        QT_MOC_LITERAL(569, 7),  // "UWBData"
        QT_MOC_LITERAL(577, 3),  // "tag"
        QT_MOC_LITERAL(581, 25),  // "onFinishedVideoProcessing"
        QT_MOC_LITERAL(607, 17),  // "onDurationUpdated"
        QT_MOC_LITERAL(625, 21),  // "currentTimeInMSeconds"
        QT_MOC_LITERAL(647, 13),  // "onVideoOpened"
        QT_MOC_LITERAL(661, 11),  // "totalFrames"
        QT_MOC_LITERAL(673, 13),  // "videoDuration"
        QT_MOC_LITERAL(687, 12),  // "onFileLoaded"
        QT_MOC_LITERAL(700, 7),  // "success"
        QT_MOC_LITERAL(708, 7),  // "message"
        QT_MOC_LITERAL(716, 19),  // "onShowExportWarning"
        QT_MOC_LITERAL(736, 6),  // "header"
        QT_MOC_LITERAL(743, 10),  // "ExportType"
        QT_MOC_LITERAL(754, 4),  // "type"
        QT_MOC_LITERAL(759, 25),  // "onShowExportProcessDialog"
        QT_MOC_LITERAL(785, 22),  // "onUpdateExportProgress"
        QT_MOC_LITERAL(808, 10),  // "proportion"
        QT_MOC_LITERAL(819, 16),  // "onExportFinished"
        QT_MOC_LITERAL(836, 16),  // "onModelNotLoaded"
        QT_MOC_LITERAL(853, 14),  // "PredictionType"
        QT_MOC_LITERAL(868, 17),  // "onPositionUpdated"
        QT_MOC_LITERAL(886, 11),  // "currentTime"
        QT_MOC_LITERAL(898, 13),  // "onShowWarning"
        QT_MOC_LITERAL(912, 28),  // "onChangePredictionButtonName"
        QT_MOC_LITERAL(941, 21),  // "isPredictionRequested"
        QT_MOC_LITERAL(963, 29),  // "onHumanDetectorNotInitialized"
        QT_MOC_LITERAL(993, 46),  // "on_actionLoad_human_detector_..."
        QT_MOC_LITERAL(1040, 42),  // "on_pushButton_UWB_Show_Coordi..."
        QT_MOC_LITERAL(1083, 46),  // "on_pushButton_Optical_Show_Co..."
        QT_MOC_LITERAL(1130, 52),  // "on_pushButton_Pixel_to_Real_S..."
        QT_MOC_LITERAL(1183, 18)   // "onDistCoeffsLoaded"
    },
    "IndoorPositioningSystemUI",
    "tagPositionUpdated",
    "",
    "position",
    "tagID",
    "on_horizontalSlider_Duration_valueChanged",
    "on_pushButton_Play_Pause_clicked",
    "on_actionOpen_Video_triggered",
    "on_horizontalSlider_Duration_sliderReleased",
    "on_pushButton_UWB_Data_Analysis_clicked",
    "on_pushButton_UWB_Localization_clicked",
    "on_pushButton_Export_Data_clicked",
    "onAcceptFrameByFrameExport",
    "on_actionLoad_model_params_triggered",
    "on_pushButton_Predict_Optical_clicked",
    "on_pushButton_Predict_Model_clicked",
    "on_actionLoad_intrinsic_calibration_params_triggered",
    "onDataUpdated",
    "image",
    "frameID",
    "timestamp",
    "onUWBDataUpdated",
    "UWBData",
    "tag",
    "onFinishedVideoProcessing",
    "onDurationUpdated",
    "currentTimeInMSeconds",
    "onVideoOpened",
    "totalFrames",
    "videoDuration",
    "onFileLoaded",
    "success",
    "message",
    "onShowExportWarning",
    "header",
    "ExportType",
    "type",
    "onShowExportProcessDialog",
    "onUpdateExportProgress",
    "proportion",
    "onExportFinished",
    "onModelNotLoaded",
    "PredictionType",
    "onPositionUpdated",
    "currentTime",
    "onShowWarning",
    "onChangePredictionButtonName",
    "isPredictionRequested",
    "onHumanDetectorNotInitialized",
    "on_actionLoad_human_detector_weights_triggered",
    "on_pushButton_UWB_Show_Coordinates_clicked",
    "on_pushButton_Optical_Show_Coordinates_clicked",
    "on_pushButton_Pixel_to_Real_Show_Coordinates_clicked",
    "onDistCoeffsLoaded"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndoorPositioningSystemUIENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,  212,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,  217,    2, 0x08,    4 /* Private */,
       6,    0,  220,    2, 0x08,    6 /* Private */,
       7,    0,  221,    2, 0x08,    7 /* Private */,
       8,    0,  222,    2, 0x08,    8 /* Private */,
       9,    0,  223,    2, 0x08,    9 /* Private */,
      10,    0,  224,    2, 0x08,   10 /* Private */,
      11,    0,  225,    2, 0x08,   11 /* Private */,
      12,    0,  226,    2, 0x08,   12 /* Private */,
      13,    0,  227,    2, 0x08,   13 /* Private */,
      14,    0,  228,    2, 0x08,   14 /* Private */,
      15,    0,  229,    2, 0x08,   15 /* Private */,
      16,    0,  230,    2, 0x08,   16 /* Private */,
      17,    3,  231,    2, 0x08,   17 /* Private */,
      21,    1,  238,    2, 0x08,   21 /* Private */,
      24,    0,  241,    2, 0x08,   23 /* Private */,
      25,    2,  242,    2, 0x08,   24 /* Private */,
      27,    2,  247,    2, 0x08,   27 /* Private */,
      30,    2,  252,    2, 0x08,   30 /* Private */,
      33,    3,  257,    2, 0x08,   33 /* Private */,
      37,    0,  264,    2, 0x08,   37 /* Private */,
      38,    1,  265,    2, 0x08,   38 /* Private */,
      40,    1,  268,    2, 0x08,   40 /* Private */,
      41,    1,  271,    2, 0x08,   42 /* Private */,
      43,    1,  274,    2, 0x08,   44 /* Private */,
      45,    2,  277,    2, 0x08,   46 /* Private */,
      46,    2,  282,    2, 0x08,   49 /* Private */,
      48,    0,  287,    2, 0x08,   52 /* Private */,
      49,    0,  288,    2, 0x08,   53 /* Private */,
      50,    0,  289,    2, 0x08,   54 /* Private */,
      51,    0,  290,    2, 0x08,   55 /* Private */,
      52,    0,  291,    2, 0x08,   56 /* Private */,
      53,    0,  292,    2, 0x08,   57 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage, QMetaType::Int, QMetaType::QString,   18,   19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::LongLong,   19,   26,
    QMetaType::Void, QMetaType::Int, QMetaType::LongLong,   28,   29,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   31,   32,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 35,   34,   32,   36,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   39,
    QMetaType::Void, QMetaType::Bool,   31,
    QMetaType::Void, 0x80000000 | 42,   36,
    QMetaType::Void, QMetaType::QString,   44,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   34,   32,
    QMetaType::Void, 0x80000000 | 42, QMetaType::Bool,   36,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject IndoorPositioningSystemUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndoorPositioningSystemUIENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<IndoorPositioningSystemUI, std::true_type>,
        // method 'tagPositionUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPointF &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_horizontalSlider_Duration_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_pushButton_Play_Pause_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionOpen_Video_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_horizontalSlider_Duration_sliderReleased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_UWB_Data_Analysis_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_UWB_Localization_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Export_Data_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAcceptFrameByFrameExport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionLoad_model_params_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Predict_Optical_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Predict_Model_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionLoad_intrinsic_calibration_params_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDataUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onUWBDataUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<UWBData, std::false_type>,
        // method 'onFinishedVideoProcessing'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDurationUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<long long, std::false_type>,
        // method 'onVideoOpened'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<long long, std::false_type>,
        // method 'onFileLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onShowExportWarning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<ExportType, std::false_type>,
        // method 'onShowExportProcessDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onUpdateExportProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onExportFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onModelNotLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PredictionType, std::false_type>,
        // method 'onPositionUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onShowWarning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onChangePredictionButtonName'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PredictionType, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onHumanDetectorNotInitialized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionLoad_human_detector_weights_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_UWB_Show_Coordinates_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Optical_Show_Coordinates_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_Pixel_to_Real_Show_Coordinates_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDistCoeffsLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void IndoorPositioningSystemUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IndoorPositioningSystemUI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tagPositionUpdated((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->on_horizontalSlider_Duration_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->on_pushButton_Play_Pause_clicked(); break;
        case 3: _t->on_actionOpen_Video_triggered(); break;
        case 4: _t->on_horizontalSlider_Duration_sliderReleased(); break;
        case 5: _t->on_pushButton_UWB_Data_Analysis_clicked(); break;
        case 6: _t->on_pushButton_UWB_Localization_clicked(); break;
        case 7: _t->on_pushButton_Export_Data_clicked(); break;
        case 8: _t->onAcceptFrameByFrameExport(); break;
        case 9: _t->on_actionLoad_model_params_triggered(); break;
        case 10: _t->on_pushButton_Predict_Optical_clicked(); break;
        case 11: _t->on_pushButton_Predict_Model_clicked(); break;
        case 12: _t->on_actionLoad_intrinsic_calibration_params_triggered(); break;
        case 13: _t->onDataUpdated((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 14: _t->onUWBDataUpdated((*reinterpret_cast< std::add_pointer_t<UWBData>>(_a[1]))); break;
        case 15: _t->onFinishedVideoProcessing(); break;
        case 16: _t->onDurationUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 17: _t->onVideoOpened((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 18: _t->onFileLoaded((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 19: _t->onShowExportWarning((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ExportType>>(_a[3]))); break;
        case 20: _t->onShowExportProcessDialog(); break;
        case 21: _t->onUpdateExportProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->onExportFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 23: _t->onModelNotLoaded((*reinterpret_cast< std::add_pointer_t<PredictionType>>(_a[1]))); break;
        case 24: _t->onPositionUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 25: _t->onShowWarning((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 26: _t->onChangePredictionButtonName((*reinterpret_cast< std::add_pointer_t<PredictionType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 27: _t->onHumanDetectorNotInitialized(); break;
        case 28: _t->on_actionLoad_human_detector_weights_triggered(); break;
        case 29: _t->on_pushButton_UWB_Show_Coordinates_clicked(); break;
        case 30: _t->on_pushButton_Optical_Show_Coordinates_clicked(); break;
        case 31: _t->on_pushButton_Pixel_to_Real_Show_Coordinates_clicked(); break;
        case 32: _t->onDistCoeffsLoaded(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IndoorPositioningSystemUI::*)(const QPointF & , int );
            if (_t _q_method = &IndoorPositioningSystemUI::tagPositionUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *IndoorPositioningSystemUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IndoorPositioningSystemUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndoorPositioningSystemUIENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int IndoorPositioningSystemUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 33)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 33;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 33)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 33;
    }
    return _id;
}

// SIGNAL 0
void IndoorPositioningSystemUI::tagPositionUpdated(const QPointF & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP

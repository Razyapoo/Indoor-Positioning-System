/****************************************************************************
** Meta object code from reading C++ file 'indoorpositioningsystemviewmodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../IndoorPositioningSystem/indoorpositioningsystemviewmodel.h"
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
#error "The header file 'indoorpositioningsystemviewmodel.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS = QtMocHelpers::stringData(
    "IndoorPositioningSystemViewModel",
    "requestProcessVideo",
    "",
    "frameIsReady",
    "UWBVideoData",
    "data",
    "dataUpdated",
    "image",
    "frameID",
    "timestamp",
    "uwbDataUpdated",
    "UWBData",
    "tag",
    "finishedVideoProcessing",
    "updateTagPosition",
    "coordinates",
    "tagID",
    "updatePixelToRealPosition",
    "objectID",
    "updateOpticalPosition",
    "durationUpdated",
    "currentTimeInMSeconds",
    "videoOpened",
    "totalFrames",
    "videoDuration",
    "modelParamsLoaded",
    "success",
    "message",
    "intrinsicCalibrationParamsLoaded",
    "showExportWarning",
    "header",
    "ExportType",
    "type",
    "showExportProcessDialog",
    "updateExportProgress",
    "proportion",
    "exportFinished",
    "modelNotLoaded",
    "PredictionType",
    "positionUpdated",
    "currentTime",
    "showWarning",
    "requestChangePredictionButtonName",
    "isPredictionRequested",
    "weightsLoaded",
    "humanDetectorNotInitialized",
    "distCoeffsLoaded",
    "requestSetRangeForDataAnalysis",
    "startTimeSec",
    "endTimeSec",
    "requestCollectDataForPlotDistancesVsTimestamps",
    "anchorID",
    "requestCalculateRollingDeviation",
    "windowSize",
    "requestSplitDataset",
    "threshold",
    "requestCalculatePolynomialRegression",
    "std::vector<double>",
    "referenceValues",
    "requestUpdateOriginalWithAdjustedValues",
    "requestCollectDataForTag",
    "tagIDText",
    "requestShowAvailableTags",
    "std::vector<int>",
    "availableTagIDs",
    "requestShowAvailableAnchors",
    "availableAnchorIDs",
    "requestShowPlotDistancesVsTimestamps",
    "std::vector<qlonglong>",
    "timestamps",
    "std::vector<double*>",
    "distances",
    "requestShowPlotRollingDeviations",
    "deviations",
    "requestShowDatasetSegments",
    "segmentMeans",
    "requestShowOriginalVsAdjustedDistances",
    "timestampsToAnalyze",
    "distancesToAnalyzeOriginal",
    "distancesToAnalyzeAdjusted",
    "checkForDisplay",
    "updateDataDisplay",
    "onExportFinished",
    "onExportProgressUpdated",
    "index",
    "onSegmentFramesExport",
    "afterSeeking",
    "onChangePredictionButtonName",
    "onHumanDetectorNotInitialized",
    "onDistCoeffsLoaded",
    "onStartTimer",
    "setRangeForDataAnalysis",
    "collectDataForPlotDistancesVsTimestamps",
    "calculateRollingDeviation",
    "splitDataset",
    "calculatePolynomialRegression",
    "updateOriginalWithAdjustedValues",
    "collectDataForTag",
    "showAvailableTags",
    "showAvailableAnchors",
    "showPlotDistancesVsTimestamps",
    "showPlotRollingDeviations",
    "showDatasetSegments",
    "datasetSegmentMeans",
    "showOriginalVsAdjustedDistances"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS_t {
    uint offsetsAndSizes[210];
    char stringdata0[33];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[13];
    char stringdata5[5];
    char stringdata6[12];
    char stringdata7[6];
    char stringdata8[8];
    char stringdata9[10];
    char stringdata10[15];
    char stringdata11[8];
    char stringdata12[4];
    char stringdata13[24];
    char stringdata14[18];
    char stringdata15[12];
    char stringdata16[6];
    char stringdata17[26];
    char stringdata18[9];
    char stringdata19[22];
    char stringdata20[16];
    char stringdata21[22];
    char stringdata22[12];
    char stringdata23[12];
    char stringdata24[14];
    char stringdata25[18];
    char stringdata26[8];
    char stringdata27[8];
    char stringdata28[33];
    char stringdata29[18];
    char stringdata30[7];
    char stringdata31[11];
    char stringdata32[5];
    char stringdata33[24];
    char stringdata34[21];
    char stringdata35[11];
    char stringdata36[15];
    char stringdata37[15];
    char stringdata38[15];
    char stringdata39[16];
    char stringdata40[12];
    char stringdata41[12];
    char stringdata42[34];
    char stringdata43[22];
    char stringdata44[14];
    char stringdata45[28];
    char stringdata46[17];
    char stringdata47[31];
    char stringdata48[13];
    char stringdata49[11];
    char stringdata50[47];
    char stringdata51[9];
    char stringdata52[33];
    char stringdata53[11];
    char stringdata54[20];
    char stringdata55[10];
    char stringdata56[37];
    char stringdata57[20];
    char stringdata58[16];
    char stringdata59[40];
    char stringdata60[25];
    char stringdata61[10];
    char stringdata62[25];
    char stringdata63[17];
    char stringdata64[16];
    char stringdata65[28];
    char stringdata66[19];
    char stringdata67[37];
    char stringdata68[23];
    char stringdata69[11];
    char stringdata70[21];
    char stringdata71[10];
    char stringdata72[33];
    char stringdata73[11];
    char stringdata74[27];
    char stringdata75[13];
    char stringdata76[39];
    char stringdata77[20];
    char stringdata78[27];
    char stringdata79[27];
    char stringdata80[16];
    char stringdata81[18];
    char stringdata82[17];
    char stringdata83[24];
    char stringdata84[6];
    char stringdata85[22];
    char stringdata86[13];
    char stringdata87[29];
    char stringdata88[30];
    char stringdata89[19];
    char stringdata90[13];
    char stringdata91[24];
    char stringdata92[40];
    char stringdata93[26];
    char stringdata94[13];
    char stringdata95[30];
    char stringdata96[33];
    char stringdata97[18];
    char stringdata98[18];
    char stringdata99[21];
    char stringdata100[30];
    char stringdata101[26];
    char stringdata102[20];
    char stringdata103[20];
    char stringdata104[32];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS_t qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 32),  // "IndoorPositioningSystemViewModel"
        QT_MOC_LITERAL(33, 19),  // "requestProcessVideo"
        QT_MOC_LITERAL(53, 0),  // ""
        QT_MOC_LITERAL(54, 12),  // "frameIsReady"
        QT_MOC_LITERAL(67, 12),  // "UWBVideoData"
        QT_MOC_LITERAL(80, 4),  // "data"
        QT_MOC_LITERAL(85, 11),  // "dataUpdated"
        QT_MOC_LITERAL(97, 5),  // "image"
        QT_MOC_LITERAL(103, 7),  // "frameID"
        QT_MOC_LITERAL(111, 9),  // "timestamp"
        QT_MOC_LITERAL(121, 14),  // "uwbDataUpdated"
        QT_MOC_LITERAL(136, 7),  // "UWBData"
        QT_MOC_LITERAL(144, 3),  // "tag"
        QT_MOC_LITERAL(148, 23),  // "finishedVideoProcessing"
        QT_MOC_LITERAL(172, 17),  // "updateTagPosition"
        QT_MOC_LITERAL(190, 11),  // "coordinates"
        QT_MOC_LITERAL(202, 5),  // "tagID"
        QT_MOC_LITERAL(208, 25),  // "updatePixelToRealPosition"
        QT_MOC_LITERAL(234, 8),  // "objectID"
        QT_MOC_LITERAL(243, 21),  // "updateOpticalPosition"
        QT_MOC_LITERAL(265, 15),  // "durationUpdated"
        QT_MOC_LITERAL(281, 21),  // "currentTimeInMSeconds"
        QT_MOC_LITERAL(303, 11),  // "videoOpened"
        QT_MOC_LITERAL(315, 11),  // "totalFrames"
        QT_MOC_LITERAL(327, 13),  // "videoDuration"
        QT_MOC_LITERAL(341, 17),  // "modelParamsLoaded"
        QT_MOC_LITERAL(359, 7),  // "success"
        QT_MOC_LITERAL(367, 7),  // "message"
        QT_MOC_LITERAL(375, 32),  // "intrinsicCalibrationParamsLoaded"
        QT_MOC_LITERAL(408, 17),  // "showExportWarning"
        QT_MOC_LITERAL(426, 6),  // "header"
        QT_MOC_LITERAL(433, 10),  // "ExportType"
        QT_MOC_LITERAL(444, 4),  // "type"
        QT_MOC_LITERAL(449, 23),  // "showExportProcessDialog"
        QT_MOC_LITERAL(473, 20),  // "updateExportProgress"
        QT_MOC_LITERAL(494, 10),  // "proportion"
        QT_MOC_LITERAL(505, 14),  // "exportFinished"
        QT_MOC_LITERAL(520, 14),  // "modelNotLoaded"
        QT_MOC_LITERAL(535, 14),  // "PredictionType"
        QT_MOC_LITERAL(550, 15),  // "positionUpdated"
        QT_MOC_LITERAL(566, 11),  // "currentTime"
        QT_MOC_LITERAL(578, 11),  // "showWarning"
        QT_MOC_LITERAL(590, 33),  // "requestChangePredictionButton..."
        QT_MOC_LITERAL(624, 21),  // "isPredictionRequested"
        QT_MOC_LITERAL(646, 13),  // "weightsLoaded"
        QT_MOC_LITERAL(660, 27),  // "humanDetectorNotInitialized"
        QT_MOC_LITERAL(688, 16),  // "distCoeffsLoaded"
        QT_MOC_LITERAL(705, 30),  // "requestSetRangeForDataAnalysis"
        QT_MOC_LITERAL(736, 12),  // "startTimeSec"
        QT_MOC_LITERAL(749, 10),  // "endTimeSec"
        QT_MOC_LITERAL(760, 46),  // "requestCollectDataForPlotDist..."
        QT_MOC_LITERAL(807, 8),  // "anchorID"
        QT_MOC_LITERAL(816, 32),  // "requestCalculateRollingDeviation"
        QT_MOC_LITERAL(849, 10),  // "windowSize"
        QT_MOC_LITERAL(860, 19),  // "requestSplitDataset"
        QT_MOC_LITERAL(880, 9),  // "threshold"
        QT_MOC_LITERAL(890, 36),  // "requestCalculatePolynomialReg..."
        QT_MOC_LITERAL(927, 19),  // "std::vector<double>"
        QT_MOC_LITERAL(947, 15),  // "referenceValues"
        QT_MOC_LITERAL(963, 39),  // "requestUpdateOriginalWithAdju..."
        QT_MOC_LITERAL(1003, 24),  // "requestCollectDataForTag"
        QT_MOC_LITERAL(1028, 9),  // "tagIDText"
        QT_MOC_LITERAL(1038, 24),  // "requestShowAvailableTags"
        QT_MOC_LITERAL(1063, 16),  // "std::vector<int>"
        QT_MOC_LITERAL(1080, 15),  // "availableTagIDs"
        QT_MOC_LITERAL(1096, 27),  // "requestShowAvailableAnchors"
        QT_MOC_LITERAL(1124, 18),  // "availableAnchorIDs"
        QT_MOC_LITERAL(1143, 36),  // "requestShowPlotDistancesVsTim..."
        QT_MOC_LITERAL(1180, 22),  // "std::vector<qlonglong>"
        QT_MOC_LITERAL(1203, 10),  // "timestamps"
        QT_MOC_LITERAL(1214, 20),  // "std::vector<double*>"
        QT_MOC_LITERAL(1235, 9),  // "distances"
        QT_MOC_LITERAL(1245, 32),  // "requestShowPlotRollingDeviations"
        QT_MOC_LITERAL(1278, 10),  // "deviations"
        QT_MOC_LITERAL(1289, 26),  // "requestShowDatasetSegments"
        QT_MOC_LITERAL(1316, 12),  // "segmentMeans"
        QT_MOC_LITERAL(1329, 38),  // "requestShowOriginalVsAdjusted..."
        QT_MOC_LITERAL(1368, 19),  // "timestampsToAnalyze"
        QT_MOC_LITERAL(1388, 26),  // "distancesToAnalyzeOriginal"
        QT_MOC_LITERAL(1415, 26),  // "distancesToAnalyzeAdjusted"
        QT_MOC_LITERAL(1442, 15),  // "checkForDisplay"
        QT_MOC_LITERAL(1458, 17),  // "updateDataDisplay"
        QT_MOC_LITERAL(1476, 16),  // "onExportFinished"
        QT_MOC_LITERAL(1493, 23),  // "onExportProgressUpdated"
        QT_MOC_LITERAL(1517, 5),  // "index"
        QT_MOC_LITERAL(1523, 21),  // "onSegmentFramesExport"
        QT_MOC_LITERAL(1545, 12),  // "afterSeeking"
        QT_MOC_LITERAL(1558, 28),  // "onChangePredictionButtonName"
        QT_MOC_LITERAL(1587, 29),  // "onHumanDetectorNotInitialized"
        QT_MOC_LITERAL(1617, 18),  // "onDistCoeffsLoaded"
        QT_MOC_LITERAL(1636, 12),  // "onStartTimer"
        QT_MOC_LITERAL(1649, 23),  // "setRangeForDataAnalysis"
        QT_MOC_LITERAL(1673, 39),  // "collectDataForPlotDistancesVs..."
        QT_MOC_LITERAL(1713, 25),  // "calculateRollingDeviation"
        QT_MOC_LITERAL(1739, 12),  // "splitDataset"
        QT_MOC_LITERAL(1752, 29),  // "calculatePolynomialRegression"
        QT_MOC_LITERAL(1782, 32),  // "updateOriginalWithAdjustedValues"
        QT_MOC_LITERAL(1815, 17),  // "collectDataForTag"
        QT_MOC_LITERAL(1833, 17),  // "showAvailableTags"
        QT_MOC_LITERAL(1851, 20),  // "showAvailableAnchors"
        QT_MOC_LITERAL(1872, 29),  // "showPlotDistancesVsTimestamps"
        QT_MOC_LITERAL(1902, 25),  // "showPlotRollingDeviations"
        QT_MOC_LITERAL(1928, 19),  // "showDatasetSegments"
        QT_MOC_LITERAL(1948, 19),  // "datasetSegmentMeans"
        QT_MOC_LITERAL(1968, 31)   // "showOriginalVsAdjustedDistances"
    },
    "IndoorPositioningSystemViewModel",
    "requestProcessVideo",
    "",
    "frameIsReady",
    "UWBVideoData",
    "data",
    "dataUpdated",
    "image",
    "frameID",
    "timestamp",
    "uwbDataUpdated",
    "UWBData",
    "tag",
    "finishedVideoProcessing",
    "updateTagPosition",
    "coordinates",
    "tagID",
    "updatePixelToRealPosition",
    "objectID",
    "updateOpticalPosition",
    "durationUpdated",
    "currentTimeInMSeconds",
    "videoOpened",
    "totalFrames",
    "videoDuration",
    "modelParamsLoaded",
    "success",
    "message",
    "intrinsicCalibrationParamsLoaded",
    "showExportWarning",
    "header",
    "ExportType",
    "type",
    "showExportProcessDialog",
    "updateExportProgress",
    "proportion",
    "exportFinished",
    "modelNotLoaded",
    "PredictionType",
    "positionUpdated",
    "currentTime",
    "showWarning",
    "requestChangePredictionButtonName",
    "isPredictionRequested",
    "weightsLoaded",
    "humanDetectorNotInitialized",
    "distCoeffsLoaded",
    "requestSetRangeForDataAnalysis",
    "startTimeSec",
    "endTimeSec",
    "requestCollectDataForPlotDistancesVsTimestamps",
    "anchorID",
    "requestCalculateRollingDeviation",
    "windowSize",
    "requestSplitDataset",
    "threshold",
    "requestCalculatePolynomialRegression",
    "std::vector<double>",
    "referenceValues",
    "requestUpdateOriginalWithAdjustedValues",
    "requestCollectDataForTag",
    "tagIDText",
    "requestShowAvailableTags",
    "std::vector<int>",
    "availableTagIDs",
    "requestShowAvailableAnchors",
    "availableAnchorIDs",
    "requestShowPlotDistancesVsTimestamps",
    "std::vector<qlonglong>",
    "timestamps",
    "std::vector<double*>",
    "distances",
    "requestShowPlotRollingDeviations",
    "deviations",
    "requestShowDatasetSegments",
    "segmentMeans",
    "requestShowOriginalVsAdjustedDistances",
    "timestampsToAnalyze",
    "distancesToAnalyzeOriginal",
    "distancesToAnalyzeAdjusted",
    "checkForDisplay",
    "updateDataDisplay",
    "onExportFinished",
    "onExportProgressUpdated",
    "index",
    "onSegmentFramesExport",
    "afterSeeking",
    "onChangePredictionButtonName",
    "onHumanDetectorNotInitialized",
    "onDistCoeffsLoaded",
    "onStartTimer",
    "setRangeForDataAnalysis",
    "collectDataForPlotDistancesVsTimestamps",
    "calculateRollingDeviation",
    "splitDataset",
    "calculatePolynomialRegression",
    "updateOriginalWithAdjustedValues",
    "collectDataForTag",
    "showAvailableTags",
    "showAvailableAnchors",
    "showPlotDistancesVsTimestamps",
    "showPlotRollingDeviations",
    "showDatasetSegments",
    "datasetSegmentMeans",
    "showOriginalVsAdjustedDistances"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSIndoorPositioningSystemViewModelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      59,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      36,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  368,    2, 0x06,    1 /* Public */,
       3,    1,  369,    2, 0x06,    2 /* Public */,
       6,    3,  372,    2, 0x06,    4 /* Public */,
      10,    1,  379,    2, 0x06,    8 /* Public */,
      13,    0,  382,    2, 0x06,   10 /* Public */,
      14,    2,  383,    2, 0x06,   11 /* Public */,
      17,    2,  388,    2, 0x06,   14 /* Public */,
      19,    2,  393,    2, 0x06,   17 /* Public */,
      20,    2,  398,    2, 0x06,   20 /* Public */,
      22,    2,  403,    2, 0x06,   23 /* Public */,
      25,    2,  408,    2, 0x06,   26 /* Public */,
      28,    2,  413,    2, 0x06,   29 /* Public */,
      29,    3,  418,    2, 0x06,   32 /* Public */,
      33,    0,  425,    2, 0x06,   36 /* Public */,
      34,    1,  426,    2, 0x06,   37 /* Public */,
      36,    1,  429,    2, 0x06,   39 /* Public */,
      37,    1,  432,    2, 0x06,   41 /* Public */,
      39,    1,  435,    2, 0x06,   43 /* Public */,
      41,    2,  438,    2, 0x06,   45 /* Public */,
      42,    2,  443,    2, 0x06,   48 /* Public */,
      44,    2,  448,    2, 0x06,   51 /* Public */,
      45,    0,  453,    2, 0x06,   54 /* Public */,
      46,    0,  454,    2, 0x06,   55 /* Public */,
      47,    2,  455,    2, 0x06,   56 /* Public */,
      50,    1,  460,    2, 0x06,   59 /* Public */,
      52,    1,  463,    2, 0x06,   61 /* Public */,
      54,    1,  466,    2, 0x06,   63 /* Public */,
      56,    1,  469,    2, 0x06,   65 /* Public */,
      59,    0,  472,    2, 0x06,   67 /* Public */,
      60,    1,  473,    2, 0x06,   68 /* Public */,
      62,    1,  476,    2, 0x06,   70 /* Public */,
      65,    1,  479,    2, 0x06,   72 /* Public */,
      67,    2,  482,    2, 0x06,   74 /* Public */,
      72,    2,  487,    2, 0x06,   77 /* Public */,
      74,    1,  492,    2, 0x06,   80 /* Public */,
      76,    3,  495,    2, 0x06,   82 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      80,    0,  502,    2, 0x0a,   86 /* Public */,
      81,    1,  503,    2, 0x0a,   87 /* Public */,
      82,    1,  506,    2, 0x0a,   89 /* Public */,
      83,    1,  509,    2, 0x0a,   91 /* Public */,
      85,    0,  512,    2, 0x0a,   93 /* Public */,
      86,    0,  513,    2, 0x0a,   94 /* Public */,
      87,    2,  514,    2, 0x0a,   95 /* Public */,
      88,    0,  519,    2, 0x0a,   98 /* Public */,
      89,    0,  520,    2, 0x0a,   99 /* Public */,
      90,    0,  521,    2, 0x0a,  100 /* Public */,
      91,    2,  522,    2, 0x0a,  101 /* Public */,
      92,    1,  527,    2, 0x0a,  104 /* Public */,
      93,    1,  530,    2, 0x0a,  106 /* Public */,
      94,    1,  533,    2, 0x0a,  108 /* Public */,
      95,    1,  536,    2, 0x0a,  110 /* Public */,
      96,    0,  539,    2, 0x0a,  112 /* Public */,
      97,    1,  540,    2, 0x0a,  113 /* Public */,
      98,    1,  543,    2, 0x0a,  115 /* Public */,
      99,    1,  546,    2, 0x0a,  117 /* Public */,
     100,    2,  549,    2, 0x0a,  119 /* Public */,
     101,    2,  554,    2, 0x0a,  122 /* Public */,
     102,    1,  559,    2, 0x0a,  125 /* Public */,
     104,    3,  562,    2, 0x0a,  127 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::QImage, QMetaType::Int, QMetaType::QString,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int,   15,   16,
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int,   15,   18,
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int,   15,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::LongLong,    8,   21,
    QMetaType::Void, QMetaType::Int, QMetaType::LongLong,   23,   24,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   26,   27,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   26,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 31,   30,   27,   32,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, 0x80000000 | 38,   32,
    QMetaType::Void, QMetaType::QString,   40,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   30,   27,
    QMetaType::Void, 0x80000000 | 38, QMetaType::Bool,   32,   43,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   26,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   48,   49,
    QMetaType::Void, QMetaType::Int,   51,
    QMetaType::Void, QMetaType::Int,   53,
    QMetaType::Void, QMetaType::Double,   55,
    QMetaType::Void, 0x80000000 | 57,   58,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   61,
    QMetaType::Void, 0x80000000 | 63,   64,
    QMetaType::Void, 0x80000000 | 63,   66,
    QMetaType::Void, 0x80000000 | 68, 0x80000000 | 70,   69,   71,
    QMetaType::Void, 0x80000000 | 68, 0x80000000 | 57,   69,   73,
    QMetaType::Void, 0x80000000 | 57,   75,
    QMetaType::Void, 0x80000000 | 68, 0x80000000 | 70, 0x80000000 | 57,   77,   78,   79,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Int,   84,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 38, QMetaType::Bool,   32,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   48,   49,
    QMetaType::Void, QMetaType::Int,   51,
    QMetaType::Void, QMetaType::Int,   53,
    QMetaType::Void, QMetaType::Double,   55,
    QMetaType::Void, 0x80000000 | 57,   58,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   61,
    QMetaType::Void, 0x80000000 | 63,   64,
    QMetaType::Void, 0x80000000 | 63,   66,
    QMetaType::Void, 0x80000000 | 68, 0x80000000 | 70,   69,   71,
    QMetaType::Void, 0x80000000 | 68, 0x80000000 | 57,   69,   73,
    QMetaType::Void, 0x80000000 | 57,  103,
    QMetaType::Void, 0x80000000 | 68, 0x80000000 | 70, 0x80000000 | 57,   77,   78,   79,

       0        // eod
};

Q_CONSTINIT const QMetaObject IndoorPositioningSystemViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSIndoorPositioningSystemViewModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<IndoorPositioningSystemViewModel, std::true_type>,
        // method 'requestProcessVideo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'frameIsReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const UWBVideoData &, std::false_type>,
        // method 'dataUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QImage &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'uwbDataUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<UWBData, std::false_type>,
        // method 'finishedVideoProcessing'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateTagPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPointF &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updatePixelToRealPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPointF &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateOpticalPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPointF &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'durationUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<long long, std::false_type>,
        // method 'videoOpened'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<long long, std::false_type>,
        // method 'modelParamsLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'intrinsicCalibrationParamsLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showExportWarning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<ExportType, std::false_type>,
        // method 'showExportProcessDialog'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateExportProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'exportFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'modelNotLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PredictionType, std::false_type>,
        // method 'positionUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showWarning'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'requestChangePredictionButtonName'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PredictionType, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'weightsLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'humanDetectorNotInitialized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'distCoeffsLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'requestSetRangeForDataAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const long long, std::false_type>,
        QtPrivate::TypeAndForceComplete<const long long, std::false_type>,
        // method 'requestCollectDataForPlotDistancesVsTimestamps'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'requestCalculateRollingDeviation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'requestSplitDataset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const double, std::false_type>,
        // method 'requestCalculatePolynomialRegression'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'requestUpdateOriginalWithAdjustedValues'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'requestCollectDataForTag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'requestShowAvailableTags'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<int> &, std::false_type>,
        // method 'requestShowAvailableAnchors'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<int> &, std::false_type>,
        // method 'requestShowPlotDistancesVsTimestamps'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<double*>, std::false_type>,
        // method 'requestShowPlotRollingDeviations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'requestShowDatasetSegments'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'requestShowOriginalVsAdjustedDistances'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<double*>, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'checkForDisplay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateDataDisplay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const UWBVideoData &, std::false_type>,
        // method 'onExportFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onExportProgressUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onSegmentFramesExport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afterSeeking'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onChangePredictionButtonName'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PredictionType, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onHumanDetectorNotInitialized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDistCoeffsLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStartTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setRangeForDataAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const long long, std::false_type>,
        QtPrivate::TypeAndForceComplete<const long long, std::false_type>,
        // method 'collectDataForPlotDistancesVsTimestamps'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'calculateRollingDeviation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
        // method 'splitDataset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const double, std::false_type>,
        // method 'calculatePolynomialRegression'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'updateOriginalWithAdjustedValues'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'collectDataForTag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'showAvailableTags'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<int> &, std::false_type>,
        // method 'showAvailableAnchors'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<int> &, std::false_type>,
        // method 'showPlotDistancesVsTimestamps'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<double*>, std::false_type>,
        // method 'showPlotRollingDeviations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'showDatasetSegments'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'showOriginalVsAdjustedDistances'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<double*>, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>
    >,
    nullptr
} };

void IndoorPositioningSystemViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IndoorPositioningSystemViewModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestProcessVideo(); break;
        case 1: _t->frameIsReady((*reinterpret_cast< std::add_pointer_t<UWBVideoData>>(_a[1]))); break;
        case 2: _t->dataUpdated((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 3: _t->uwbDataUpdated((*reinterpret_cast< std::add_pointer_t<UWBData>>(_a[1]))); break;
        case 4: _t->finishedVideoProcessing(); break;
        case 5: _t->updateTagPosition((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->updatePixelToRealPosition((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->updateOpticalPosition((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->durationUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 9: _t->videoOpened((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 10: _t->modelParamsLoaded((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->intrinsicCalibrationParamsLoaded((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->showExportWarning((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ExportType>>(_a[3]))); break;
        case 13: _t->showExportProcessDialog(); break;
        case 14: _t->updateExportProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->exportFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 16: _t->modelNotLoaded((*reinterpret_cast< std::add_pointer_t<PredictionType>>(_a[1]))); break;
        case 17: _t->positionUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->showWarning((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 19: _t->requestChangePredictionButtonName((*reinterpret_cast< std::add_pointer_t<PredictionType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 20: _t->weightsLoaded((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->humanDetectorNotInitialized(); break;
        case 22: _t->distCoeffsLoaded(); break;
        case 23: _t->requestSetRangeForDataAnalysis((*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 24: _t->requestCollectDataForPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->requestCalculateRollingDeviation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->requestSplitDataset((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 27: _t->requestCalculatePolynomialRegression((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 28: _t->requestUpdateOriginalWithAdjustedValues(); break;
        case 29: _t->requestCollectDataForTag((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 30: _t->requestShowAvailableTags((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 31: _t->requestShowAvailableAnchors((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 32: _t->requestShowPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2]))); break;
        case 33: _t->requestShowPlotRollingDeviations((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[2]))); break;
        case 34: _t->requestShowDatasetSegments((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 35: _t->requestShowOriginalVsAdjustedDistances((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[3]))); break;
        case 36: _t->checkForDisplay(); break;
        case 37: _t->updateDataDisplay((*reinterpret_cast< std::add_pointer_t<UWBVideoData>>(_a[1]))); break;
        case 38: _t->onExportFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 39: _t->onExportProgressUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 40: _t->onSegmentFramesExport(); break;
        case 41: _t->afterSeeking(); break;
        case 42: _t->onChangePredictionButtonName((*reinterpret_cast< std::add_pointer_t<PredictionType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 43: _t->onHumanDetectorNotInitialized(); break;
        case 44: _t->onDistCoeffsLoaded(); break;
        case 45: _t->onStartTimer(); break;
        case 46: _t->setRangeForDataAnalysis((*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 47: _t->collectDataForPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 48: _t->calculateRollingDeviation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 49: _t->splitDataset((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 50: _t->calculatePolynomialRegression((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 51: _t->updateOriginalWithAdjustedValues(); break;
        case 52: _t->collectDataForTag((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 53: _t->showAvailableTags((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 54: _t->showAvailableAnchors((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 55: _t->showPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2]))); break;
        case 56: _t->showPlotRollingDeviations((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[2]))); break;
        case 57: _t->showDatasetSegments((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 58: _t->showOriginalVsAdjustedDistances((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IndoorPositioningSystemViewModel::*)();
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestProcessVideo; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const UWBVideoData & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::frameIsReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QImage & , int , const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::dataUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(UWBData );
            if (_t _q_method = &IndoorPositioningSystemViewModel::uwbDataUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)();
            if (_t _q_method = &IndoorPositioningSystemViewModel::finishedVideoProcessing; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QPointF & , int );
            if (_t _q_method = &IndoorPositioningSystemViewModel::updateTagPosition; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QPointF & , int );
            if (_t _q_method = &IndoorPositioningSystemViewModel::updatePixelToRealPosition; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QPointF & , int );
            if (_t _q_method = &IndoorPositioningSystemViewModel::updateOpticalPosition; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(int , long long );
            if (_t _q_method = &IndoorPositioningSystemViewModel::durationUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(int , long long );
            if (_t _q_method = &IndoorPositioningSystemViewModel::videoOpened; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(bool , const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::modelParamsLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(bool , const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::intrinsicCalibrationParamsLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QString & , const QString & , ExportType );
            if (_t _q_method = &IndoorPositioningSystemViewModel::showExportWarning; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)();
            if (_t _q_method = &IndoorPositioningSystemViewModel::showExportProcessDialog; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(int );
            if (_t _q_method = &IndoorPositioningSystemViewModel::updateExportProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(bool );
            if (_t _q_method = &IndoorPositioningSystemViewModel::exportFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(PredictionType );
            if (_t _q_method = &IndoorPositioningSystemViewModel::modelNotLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::positionUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QString & , const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::showWarning; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(PredictionType , bool );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestChangePredictionButtonName; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(bool , const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::weightsLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)();
            if (_t _q_method = &IndoorPositioningSystemViewModel::humanDetectorNotInitialized; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)();
            if (_t _q_method = &IndoorPositioningSystemViewModel::distCoeffsLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const long long , const long long );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestSetRangeForDataAnalysis; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const int );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestCollectDataForPlotDistancesVsTimestamps; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const int );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestCalculateRollingDeviation; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const double );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestSplitDataset; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<double> & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestCalculatePolynomialRegression; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)();
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestUpdateOriginalWithAdjustedValues; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const QString & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestCollectDataForTag; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<int> & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestShowAvailableTags; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 30;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<int> & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestShowAvailableAnchors; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 31;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<long long> & , std::vector<double*> );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestShowPlotDistancesVsTimestamps; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 32;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<long long> & , const std::vector<double> & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestShowPlotRollingDeviations; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 33;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<double> & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestShowDatasetSegments; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 34;
                return;
            }
        }
        {
            using _t = void (IndoorPositioningSystemViewModel::*)(const std::vector<long long> & , std::vector<double*> , const std::vector<double> & );
            if (_t _q_method = &IndoorPositioningSystemViewModel::requestShowOriginalVsAdjustedDistances; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 35;
                return;
            }
        }
    }
}

const QMetaObject *IndoorPositioningSystemViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IndoorPositioningSystemViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSIndoorPositioningSystemViewModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IndoorPositioningSystemViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 59)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 59;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 59)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 59;
    }
    return _id;
}

// SIGNAL 0
void IndoorPositioningSystemViewModel::requestProcessVideo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void IndoorPositioningSystemViewModel::frameIsReady(const UWBVideoData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IndoorPositioningSystemViewModel::dataUpdated(const QImage & _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IndoorPositioningSystemViewModel::uwbDataUpdated(UWBData _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void IndoorPositioningSystemViewModel::finishedVideoProcessing()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void IndoorPositioningSystemViewModel::updateTagPosition(const QPointF & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void IndoorPositioningSystemViewModel::updatePixelToRealPosition(const QPointF & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void IndoorPositioningSystemViewModel::updateOpticalPosition(const QPointF & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void IndoorPositioningSystemViewModel::durationUpdated(int _t1, long long _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void IndoorPositioningSystemViewModel::videoOpened(int _t1, long long _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void IndoorPositioningSystemViewModel::modelParamsLoaded(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void IndoorPositioningSystemViewModel::intrinsicCalibrationParamsLoaded(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void IndoorPositioningSystemViewModel::showExportWarning(const QString & _t1, const QString & _t2, ExportType _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void IndoorPositioningSystemViewModel::showExportProcessDialog()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void IndoorPositioningSystemViewModel::updateExportProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void IndoorPositioningSystemViewModel::exportFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void IndoorPositioningSystemViewModel::modelNotLoaded(PredictionType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void IndoorPositioningSystemViewModel::positionUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void IndoorPositioningSystemViewModel::showWarning(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void IndoorPositioningSystemViewModel::requestChangePredictionButtonName(PredictionType _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void IndoorPositioningSystemViewModel::weightsLoaded(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void IndoorPositioningSystemViewModel::humanDetectorNotInitialized()
{
    QMetaObject::activate(this, &staticMetaObject, 21, nullptr);
}

// SIGNAL 22
void IndoorPositioningSystemViewModel::distCoeffsLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 22, nullptr);
}

// SIGNAL 23
void IndoorPositioningSystemViewModel::requestSetRangeForDataAnalysis(const long long _t1, const long long _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void IndoorPositioningSystemViewModel::requestCollectDataForPlotDistancesVsTimestamps(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void IndoorPositioningSystemViewModel::requestCalculateRollingDeviation(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void IndoorPositioningSystemViewModel::requestSplitDataset(const double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void IndoorPositioningSystemViewModel::requestCalculatePolynomialRegression(const std::vector<double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void IndoorPositioningSystemViewModel::requestUpdateOriginalWithAdjustedValues()
{
    QMetaObject::activate(this, &staticMetaObject, 28, nullptr);
}

// SIGNAL 29
void IndoorPositioningSystemViewModel::requestCollectDataForTag(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 29, _a);
}

// SIGNAL 30
void IndoorPositioningSystemViewModel::requestShowAvailableTags(const std::vector<int> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 30, _a);
}

// SIGNAL 31
void IndoorPositioningSystemViewModel::requestShowAvailableAnchors(const std::vector<int> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 31, _a);
}

// SIGNAL 32
void IndoorPositioningSystemViewModel::requestShowPlotDistancesVsTimestamps(const std::vector<long long> & _t1, std::vector<double*> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 32, _a);
}

// SIGNAL 33
void IndoorPositioningSystemViewModel::requestShowPlotRollingDeviations(const std::vector<long long> & _t1, const std::vector<double> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 33, _a);
}

// SIGNAL 34
void IndoorPositioningSystemViewModel::requestShowDatasetSegments(const std::vector<double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 34, _a);
}

// SIGNAL 35
void IndoorPositioningSystemViewModel::requestShowOriginalVsAdjustedDistances(const std::vector<long long> & _t1, std::vector<double*> _t2, const std::vector<double> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 35, _a);
}
QT_WARNING_POP

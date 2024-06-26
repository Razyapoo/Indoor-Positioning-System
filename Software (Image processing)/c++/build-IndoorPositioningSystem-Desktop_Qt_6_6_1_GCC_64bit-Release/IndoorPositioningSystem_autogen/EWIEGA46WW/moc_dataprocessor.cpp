/****************************************************************************
** Meta object code from reading C++ file 'dataprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../IndoorPositioningSystem/dataprocessor.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataprocessor.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSDataProcessorENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDataProcessorENDCLASS = QtMocHelpers::stringData(
    "DataProcessor",
    "requestShowPlot",
    "",
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
    "std::vector<double>",
    "deviations",
    "requestShowDatasetSegments",
    "segmentMeans",
    "requestShowOriginalVsAdjustedDistances",
    "timestampsToAnalyze",
    "distancesToAnalyzeOriginal",
    "distancesToAnalyzeAdjusted",
    "exportProgressUpdated",
    "progress",
    "requestChangePredictionButtonName",
    "PredictionType",
    "type",
    "isPredictionRequested",
    "onFindUWBMeasurementAndEnqueue",
    "frameIndex",
    "qImage",
    "DetectionData",
    "detectionData",
    "onFindUWBMeasurementAndExport",
    "rangeIndex",
    "ExportType",
    "lastRecord",
    "collectDataForTag",
    "tagIDText",
    "setRangeForDataAnalysis",
    "startTimeSec",
    "endTimeSec",
    "collectDataForPlotDistancesVsTimestamps",
    "anchorID",
    "calculateRollingDeviation",
    "windowSize",
    "splitDataset",
    "threshold",
    "calculatePolynomialRegression",
    "referenceValues",
    "updateOriginalWithAdjustedValues",
    "calculateUWBCoordinates",
    "UWBData&",
    "data",
    "getSegmentFrameIDs",
    "setAnchorPositions",
    "std::vector<AnchorPosition>",
    "anchorPositions",
    "cleanup"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDataProcessorENDCLASS_t {
    uint offsetsAndSizes[118];
    char stringdata0[14];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[25];
    char stringdata4[17];
    char stringdata5[16];
    char stringdata6[28];
    char stringdata7[19];
    char stringdata8[37];
    char stringdata9[23];
    char stringdata10[11];
    char stringdata11[21];
    char stringdata12[10];
    char stringdata13[33];
    char stringdata14[20];
    char stringdata15[11];
    char stringdata16[27];
    char stringdata17[13];
    char stringdata18[39];
    char stringdata19[20];
    char stringdata20[27];
    char stringdata21[27];
    char stringdata22[22];
    char stringdata23[9];
    char stringdata24[34];
    char stringdata25[15];
    char stringdata26[5];
    char stringdata27[22];
    char stringdata28[31];
    char stringdata29[11];
    char stringdata30[7];
    char stringdata31[14];
    char stringdata32[14];
    char stringdata33[30];
    char stringdata34[11];
    char stringdata35[11];
    char stringdata36[11];
    char stringdata37[18];
    char stringdata38[10];
    char stringdata39[24];
    char stringdata40[13];
    char stringdata41[11];
    char stringdata42[40];
    char stringdata43[9];
    char stringdata44[26];
    char stringdata45[11];
    char stringdata46[13];
    char stringdata47[10];
    char stringdata48[30];
    char stringdata49[16];
    char stringdata50[33];
    char stringdata51[24];
    char stringdata52[9];
    char stringdata53[5];
    char stringdata54[19];
    char stringdata55[19];
    char stringdata56[28];
    char stringdata57[16];
    char stringdata58[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDataProcessorENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDataProcessorENDCLASS_t qt_meta_stringdata_CLASSDataProcessorENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "DataProcessor"
        QT_MOC_LITERAL(14, 15),  // "requestShowPlot"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 24),  // "requestShowAvailableTags"
        QT_MOC_LITERAL(56, 16),  // "std::vector<int>"
        QT_MOC_LITERAL(73, 15),  // "availableTagIDs"
        QT_MOC_LITERAL(89, 27),  // "requestShowAvailableAnchors"
        QT_MOC_LITERAL(117, 18),  // "availableAnchorIDs"
        QT_MOC_LITERAL(136, 36),  // "requestShowPlotDistancesVsTim..."
        QT_MOC_LITERAL(173, 22),  // "std::vector<qlonglong>"
        QT_MOC_LITERAL(196, 10),  // "timestamps"
        QT_MOC_LITERAL(207, 20),  // "std::vector<double*>"
        QT_MOC_LITERAL(228, 9),  // "distances"
        QT_MOC_LITERAL(238, 32),  // "requestShowPlotRollingDeviations"
        QT_MOC_LITERAL(271, 19),  // "std::vector<double>"
        QT_MOC_LITERAL(291, 10),  // "deviations"
        QT_MOC_LITERAL(302, 26),  // "requestShowDatasetSegments"
        QT_MOC_LITERAL(329, 12),  // "segmentMeans"
        QT_MOC_LITERAL(342, 38),  // "requestShowOriginalVsAdjusted..."
        QT_MOC_LITERAL(381, 19),  // "timestampsToAnalyze"
        QT_MOC_LITERAL(401, 26),  // "distancesToAnalyzeOriginal"
        QT_MOC_LITERAL(428, 26),  // "distancesToAnalyzeAdjusted"
        QT_MOC_LITERAL(455, 21),  // "exportProgressUpdated"
        QT_MOC_LITERAL(477, 8),  // "progress"
        QT_MOC_LITERAL(486, 33),  // "requestChangePredictionButton..."
        QT_MOC_LITERAL(520, 14),  // "PredictionType"
        QT_MOC_LITERAL(535, 4),  // "type"
        QT_MOC_LITERAL(540, 21),  // "isPredictionRequested"
        QT_MOC_LITERAL(562, 30),  // "onFindUWBMeasurementAndEnqueue"
        QT_MOC_LITERAL(593, 10),  // "frameIndex"
        QT_MOC_LITERAL(604, 6),  // "qImage"
        QT_MOC_LITERAL(611, 13),  // "DetectionData"
        QT_MOC_LITERAL(625, 13),  // "detectionData"
        QT_MOC_LITERAL(639, 29),  // "onFindUWBMeasurementAndExport"
        QT_MOC_LITERAL(669, 10),  // "rangeIndex"
        QT_MOC_LITERAL(680, 10),  // "ExportType"
        QT_MOC_LITERAL(691, 10),  // "lastRecord"
        QT_MOC_LITERAL(702, 17),  // "collectDataForTag"
        QT_MOC_LITERAL(720, 9),  // "tagIDText"
        QT_MOC_LITERAL(730, 23),  // "setRangeForDataAnalysis"
        QT_MOC_LITERAL(754, 12),  // "startTimeSec"
        QT_MOC_LITERAL(767, 10),  // "endTimeSec"
        QT_MOC_LITERAL(778, 39),  // "collectDataForPlotDistancesVs..."
        QT_MOC_LITERAL(818, 8),  // "anchorID"
        QT_MOC_LITERAL(827, 25),  // "calculateRollingDeviation"
        QT_MOC_LITERAL(853, 10),  // "windowSize"
        QT_MOC_LITERAL(864, 12),  // "splitDataset"
        QT_MOC_LITERAL(877, 9),  // "threshold"
        QT_MOC_LITERAL(887, 29),  // "calculatePolynomialRegression"
        QT_MOC_LITERAL(917, 15),  // "referenceValues"
        QT_MOC_LITERAL(933, 32),  // "updateOriginalWithAdjustedValues"
        QT_MOC_LITERAL(966, 23),  // "calculateUWBCoordinates"
        QT_MOC_LITERAL(990, 8),  // "UWBData&"
        QT_MOC_LITERAL(999, 4),  // "data"
        QT_MOC_LITERAL(1004, 18),  // "getSegmentFrameIDs"
        QT_MOC_LITERAL(1023, 18),  // "setAnchorPositions"
        QT_MOC_LITERAL(1042, 27),  // "std::vector<AnchorPosition>"
        QT_MOC_LITERAL(1070, 15),  // "anchorPositions"
        QT_MOC_LITERAL(1086, 7)   // "cleanup"
    },
    "DataProcessor",
    "requestShowPlot",
    "",
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
    "std::vector<double>",
    "deviations",
    "requestShowDatasetSegments",
    "segmentMeans",
    "requestShowOriginalVsAdjustedDistances",
    "timestampsToAnalyze",
    "distancesToAnalyzeOriginal",
    "distancesToAnalyzeAdjusted",
    "exportProgressUpdated",
    "progress",
    "requestChangePredictionButtonName",
    "PredictionType",
    "type",
    "isPredictionRequested",
    "onFindUWBMeasurementAndEnqueue",
    "frameIndex",
    "qImage",
    "DetectionData",
    "detectionData",
    "onFindUWBMeasurementAndExport",
    "rangeIndex",
    "ExportType",
    "lastRecord",
    "collectDataForTag",
    "tagIDText",
    "setRangeForDataAnalysis",
    "startTimeSec",
    "endTimeSec",
    "collectDataForPlotDistancesVsTimestamps",
    "anchorID",
    "calculateRollingDeviation",
    "windowSize",
    "splitDataset",
    "threshold",
    "calculatePolynomialRegression",
    "referenceValues",
    "updateOriginalWithAdjustedValues",
    "calculateUWBCoordinates",
    "UWBData&",
    "data",
    "getSegmentFrameIDs",
    "setAnchorPositions",
    "std::vector<AnchorPosition>",
    "anchorPositions",
    "cleanup"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDataProcessorENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  146,    2, 0x06,    1 /* Public */,
       3,    1,  147,    2, 0x06,    2 /* Public */,
       6,    1,  150,    2, 0x06,    4 /* Public */,
       8,    2,  153,    2, 0x06,    6 /* Public */,
      13,    2,  158,    2, 0x06,    9 /* Public */,
      16,    1,  163,    2, 0x06,   12 /* Public */,
      18,    3,  166,    2, 0x06,   14 /* Public */,
      22,    1,  173,    2, 0x06,   18 /* Public */,
      24,    2,  176,    2, 0x06,   20 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      28,    3,  181,    2, 0x0a,   23 /* Public */,
      33,    5,  188,    2, 0x0a,   27 /* Public */,
      37,    1,  199,    2, 0x0a,   33 /* Public */,
      39,    2,  202,    2, 0x0a,   35 /* Public */,
      42,    1,  207,    2, 0x0a,   38 /* Public */,
      44,    1,  210,    2, 0x0a,   40 /* Public */,
      46,    1,  213,    2, 0x0a,   42 /* Public */,
      48,    1,  216,    2, 0x0a,   44 /* Public */,
      50,    0,  219,    2, 0x0a,   46 /* Public */,
      51,    1,  220,    2, 0x0a,   47 /* Public */,
      54,    0,  223,    2, 0x0a,   49 /* Public */,
      55,    1,  224,    2, 0x0a,   50 /* Public */,
      58,    0,  227,    2, 0x08,   52 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    7,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,   12,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 14,   10,   15,
    QMetaType::Void, 0x80000000 | 14,   17,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11, 0x80000000 | 14,   19,   20,   21,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, 0x80000000 | 25, QMetaType::Bool,   26,   27,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QImage, 0x80000000 | 31,   29,   30,   32,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 35, 0x80000000 | 31, QMetaType::Bool,   29,   34,   26,   32,   36,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   40,   41,
    QMetaType::Void, QMetaType::Int,   43,
    QMetaType::Void, QMetaType::Int,   45,
    QMetaType::Void, QMetaType::Double,   47,
    QMetaType::Void, 0x80000000 | 14,   49,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 52,   53,
    0x80000000 | 4,
    QMetaType::Void, 0x80000000 | 56,   57,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject DataProcessor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDataProcessorENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDataProcessorENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDataProcessorENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DataProcessor, std::true_type>,
        // method 'requestShowPlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
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
        // method 'exportProgressUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'requestChangePredictionButtonName'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PredictionType, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onFindUWBMeasurementAndEnqueue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QImage, std::false_type>,
        QtPrivate::TypeAndForceComplete<DetectionData, std::false_type>,
        // method 'onFindUWBMeasurementAndExport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<ExportType, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DetectionData &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'collectDataForTag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
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
        // method 'calculateUWBCoordinates'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<UWBData &, std::false_type>,
        // method 'getSegmentFrameIDs'
        QtPrivate::TypeAndForceComplete<std::vector<int>, std::false_type>,
        // method 'setAnchorPositions'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<AnchorPosition>, std::false_type>,
        // method 'cleanup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void DataProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataProcessor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestShowPlot(); break;
        case 1: _t->requestShowAvailableTags((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 2: _t->requestShowAvailableAnchors((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 3: _t->requestShowPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2]))); break;
        case 4: _t->requestShowPlotRollingDeviations((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[2]))); break;
        case 5: _t->requestShowDatasetSegments((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 6: _t->requestShowOriginalVsAdjustedDistances((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[3]))); break;
        case 7: _t->exportProgressUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->requestChangePredictionButtonName((*reinterpret_cast< std::add_pointer_t<PredictionType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 9: _t->onFindUWBMeasurementAndEnqueue((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QImage>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<DetectionData>>(_a[3]))); break;
        case 10: _t->onFindUWBMeasurementAndExport((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ExportType>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<DetectionData>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[5]))); break;
        case 11: _t->collectDataForTag((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->setRangeForDataAnalysis((*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 13: _t->collectDataForPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->calculateRollingDeviation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->splitDataset((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 16: _t->calculatePolynomialRegression((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 17: _t->updateOriginalWithAdjustedValues(); break;
        case 18: _t->calculateUWBCoordinates((*reinterpret_cast< std::add_pointer_t<UWBData&>>(_a[1]))); break;
        case 19: { std::vector<int> _r = _t->getSegmentFrameIDs();
            if (_a[0]) *reinterpret_cast< std::vector<int>*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->setAnchorPositions((*reinterpret_cast< std::add_pointer_t<std::vector<AnchorPosition>>>(_a[1]))); break;
        case 21: _t->cleanup(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataProcessor::*)();
            if (_t _q_method = &DataProcessor::requestShowPlot; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(const std::vector<int> & );
            if (_t _q_method = &DataProcessor::requestShowAvailableTags; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(const std::vector<int> & );
            if (_t _q_method = &DataProcessor::requestShowAvailableAnchors; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(const std::vector<long long> & , std::vector<double*> );
            if (_t _q_method = &DataProcessor::requestShowPlotDistancesVsTimestamps; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(const std::vector<long long> & , const std::vector<double> & );
            if (_t _q_method = &DataProcessor::requestShowPlotRollingDeviations; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(const std::vector<double> & );
            if (_t _q_method = &DataProcessor::requestShowDatasetSegments; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(const std::vector<long long> & , std::vector<double*> , const std::vector<double> & );
            if (_t _q_method = &DataProcessor::requestShowOriginalVsAdjustedDistances; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(int );
            if (_t _q_method = &DataProcessor::exportProgressUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (DataProcessor::*)(PredictionType , bool );
            if (_t _q_method = &DataProcessor::requestChangePredictionButtonName; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *DataProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDataProcessorENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void DataProcessor::requestShowPlot()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DataProcessor::requestShowAvailableTags(const std::vector<int> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataProcessor::requestShowAvailableAnchors(const std::vector<int> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataProcessor::requestShowPlotDistancesVsTimestamps(const std::vector<long long> & _t1, std::vector<double*> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataProcessor::requestShowPlotRollingDeviations(const std::vector<long long> & _t1, const std::vector<double> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DataProcessor::requestShowDatasetSegments(const std::vector<double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DataProcessor::requestShowOriginalVsAdjustedDistances(const std::vector<long long> & _t1, std::vector<double*> _t2, const std::vector<double> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DataProcessor::exportProgressUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DataProcessor::requestChangePredictionButtonName(PredictionType _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP

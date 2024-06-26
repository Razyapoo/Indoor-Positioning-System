/****************************************************************************
** Meta object code from reading C++ file 'dataanalysiswindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../IndoorPositioningSystem/dataanalysiswindow.h"
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
#error "The header file 'dataanalysiswindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS = QtMocHelpers::stringData(
    "DataAnalysisWindow",
    "requestAnalyseData",
    "",
    "startTimeSec",
    "endTimeSec",
    "requestCollectDataForTag",
    "tagID",
    "requestCollectDataForAnchor",
    "anchorID",
    "requestCollectDataForPlotDistancesVsTimestamps",
    "requestCalculateRollingDeviation",
    "windowSize",
    "requestSplitDataset",
    "threshold",
    "requestCalculatePolynomialRegression",
    "std::vector<double>",
    "referenceValues",
    "requestUpdateOriginalWithAdjustedValues",
    "requestSegmentFramesExport",
    "showAvailableTags",
    "std::vector<int>",
    "availableTagIDs",
    "showAvailableAnchors",
    "availableAnchorIDs",
    "showPlotDistancesVsTimestamps",
    "std::vector<qlonglong>",
    "timestamps",
    "std::vector<double*>",
    "distances",
    "initDataAnalysis",
    "startDataAnalysis",
    "showPlotRollingDeviations",
    "deviations",
    "validateRollingDeviationInput",
    "validateThresholdInput",
    "showDatasetSegments",
    "datasetSegmentMeans",
    "showOriginalVsAdjustedDistances",
    "timestampsToAnalyze",
    "distancesToAnalyzeOriginal",
    "distancesToAnalyzeAdjusted",
    "uploadReferenceValues"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS_t {
    uint offsetsAndSizes[84];
    char stringdata0[19];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[11];
    char stringdata5[25];
    char stringdata6[6];
    char stringdata7[28];
    char stringdata8[9];
    char stringdata9[47];
    char stringdata10[33];
    char stringdata11[11];
    char stringdata12[20];
    char stringdata13[10];
    char stringdata14[37];
    char stringdata15[20];
    char stringdata16[16];
    char stringdata17[40];
    char stringdata18[27];
    char stringdata19[18];
    char stringdata20[17];
    char stringdata21[16];
    char stringdata22[21];
    char stringdata23[19];
    char stringdata24[30];
    char stringdata25[23];
    char stringdata26[11];
    char stringdata27[21];
    char stringdata28[10];
    char stringdata29[17];
    char stringdata30[18];
    char stringdata31[26];
    char stringdata32[11];
    char stringdata33[30];
    char stringdata34[23];
    char stringdata35[20];
    char stringdata36[20];
    char stringdata37[32];
    char stringdata38[20];
    char stringdata39[27];
    char stringdata40[27];
    char stringdata41[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS_t qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 18),  // "DataAnalysisWindow"
        QT_MOC_LITERAL(19, 18),  // "requestAnalyseData"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 12),  // "startTimeSec"
        QT_MOC_LITERAL(52, 10),  // "endTimeSec"
        QT_MOC_LITERAL(63, 24),  // "requestCollectDataForTag"
        QT_MOC_LITERAL(88, 5),  // "tagID"
        QT_MOC_LITERAL(94, 27),  // "requestCollectDataForAnchor"
        QT_MOC_LITERAL(122, 8),  // "anchorID"
        QT_MOC_LITERAL(131, 46),  // "requestCollectDataForPlotDist..."
        QT_MOC_LITERAL(178, 32),  // "requestCalculateRollingDeviation"
        QT_MOC_LITERAL(211, 10),  // "windowSize"
        QT_MOC_LITERAL(222, 19),  // "requestSplitDataset"
        QT_MOC_LITERAL(242, 9),  // "threshold"
        QT_MOC_LITERAL(252, 36),  // "requestCalculatePolynomialReg..."
        QT_MOC_LITERAL(289, 19),  // "std::vector<double>"
        QT_MOC_LITERAL(309, 15),  // "referenceValues"
        QT_MOC_LITERAL(325, 39),  // "requestUpdateOriginalWithAdju..."
        QT_MOC_LITERAL(365, 26),  // "requestSegmentFramesExport"
        QT_MOC_LITERAL(392, 17),  // "showAvailableTags"
        QT_MOC_LITERAL(410, 16),  // "std::vector<int>"
        QT_MOC_LITERAL(427, 15),  // "availableTagIDs"
        QT_MOC_LITERAL(443, 20),  // "showAvailableAnchors"
        QT_MOC_LITERAL(464, 18),  // "availableAnchorIDs"
        QT_MOC_LITERAL(483, 29),  // "showPlotDistancesVsTimestamps"
        QT_MOC_LITERAL(513, 22),  // "std::vector<qlonglong>"
        QT_MOC_LITERAL(536, 10),  // "timestamps"
        QT_MOC_LITERAL(547, 20),  // "std::vector<double*>"
        QT_MOC_LITERAL(568, 9),  // "distances"
        QT_MOC_LITERAL(578, 16),  // "initDataAnalysis"
        QT_MOC_LITERAL(595, 17),  // "startDataAnalysis"
        QT_MOC_LITERAL(613, 25),  // "showPlotRollingDeviations"
        QT_MOC_LITERAL(639, 10),  // "deviations"
        QT_MOC_LITERAL(650, 29),  // "validateRollingDeviationInput"
        QT_MOC_LITERAL(680, 22),  // "validateThresholdInput"
        QT_MOC_LITERAL(703, 19),  // "showDatasetSegments"
        QT_MOC_LITERAL(723, 19),  // "datasetSegmentMeans"
        QT_MOC_LITERAL(743, 31),  // "showOriginalVsAdjustedDistances"
        QT_MOC_LITERAL(775, 19),  // "timestampsToAnalyze"
        QT_MOC_LITERAL(795, 26),  // "distancesToAnalyzeOriginal"
        QT_MOC_LITERAL(822, 26),  // "distancesToAnalyzeAdjusted"
        QT_MOC_LITERAL(849, 21)   // "uploadReferenceValues"
    },
    "DataAnalysisWindow",
    "requestAnalyseData",
    "",
    "startTimeSec",
    "endTimeSec",
    "requestCollectDataForTag",
    "tagID",
    "requestCollectDataForAnchor",
    "anchorID",
    "requestCollectDataForPlotDistancesVsTimestamps",
    "requestCalculateRollingDeviation",
    "windowSize",
    "requestSplitDataset",
    "threshold",
    "requestCalculatePolynomialRegression",
    "std::vector<double>",
    "referenceValues",
    "requestUpdateOriginalWithAdjustedValues",
    "requestSegmentFramesExport",
    "showAvailableTags",
    "std::vector<int>",
    "availableTagIDs",
    "showAvailableAnchors",
    "availableAnchorIDs",
    "showPlotDistancesVsTimestamps",
    "std::vector<qlonglong>",
    "timestamps",
    "std::vector<double*>",
    "distances",
    "initDataAnalysis",
    "startDataAnalysis",
    "showPlotRollingDeviations",
    "deviations",
    "validateRollingDeviationInput",
    "validateThresholdInput",
    "showDatasetSegments",
    "datasetSegmentMeans",
    "showOriginalVsAdjustedDistances",
    "timestampsToAnalyze",
    "distancesToAnalyzeOriginal",
    "distancesToAnalyzeAdjusted",
    "uploadReferenceValues"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDataAnalysisWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,  134,    2, 0x06,    1 /* Public */,
       5,    1,  139,    2, 0x06,    4 /* Public */,
       7,    1,  142,    2, 0x06,    6 /* Public */,
       9,    1,  145,    2, 0x06,    8 /* Public */,
      10,    1,  148,    2, 0x06,   10 /* Public */,
      12,    1,  151,    2, 0x06,   12 /* Public */,
      14,    1,  154,    2, 0x06,   14 /* Public */,
      17,    0,  157,    2, 0x06,   16 /* Public */,
      18,    0,  158,    2, 0x06,   17 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      19,    1,  159,    2, 0x0a,   18 /* Public */,
      22,    1,  162,    2, 0x0a,   20 /* Public */,
      24,    2,  165,    2, 0x0a,   22 /* Public */,
      29,    0,  170,    2, 0x0a,   25 /* Public */,
      30,    0,  171,    2, 0x0a,   26 /* Public */,
      31,    2,  172,    2, 0x0a,   27 /* Public */,
      33,    0,  177,    2, 0x0a,   30 /* Public */,
      34,    0,  178,    2, 0x0a,   31 /* Public */,
      35,    1,  179,    2, 0x0a,   32 /* Public */,
      37,    3,  182,    2, 0x0a,   34 /* Public */,
      41,    0,  189,    2, 0x0a,   38 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 20,   23,
    QMetaType::Void, 0x80000000 | 25, 0x80000000 | 27,   26,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25, 0x80000000 | 15,   26,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   36,
    QMetaType::Void, 0x80000000 | 25, 0x80000000 | 27, 0x80000000 | 15,   38,   39,   40,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject DataAnalysisWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDataAnalysisWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DataAnalysisWindow, std::true_type>,
        // method 'requestAnalyseData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const long long, std::false_type>,
        QtPrivate::TypeAndForceComplete<const long long, std::false_type>,
        // method 'requestCollectDataForTag'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'requestCollectDataForAnchor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const int, std::false_type>,
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
        // method 'requestSegmentFramesExport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
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
        // method 'initDataAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'startDataAnalysis'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showPlotRollingDeviations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'validateRollingDeviationInput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'validateThresholdInput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showDatasetSegments'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'showOriginalVsAdjustedDistances'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<long long> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::vector<double*>, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<double> &, std::false_type>,
        // method 'uploadReferenceValues'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void DataAnalysisWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataAnalysisWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestAnalyseData((*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<qlonglong>>(_a[2]))); break;
        case 1: _t->requestCollectDataForTag((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->requestCollectDataForAnchor((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->requestCollectDataForPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->requestCalculateRollingDeviation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->requestSplitDataset((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->requestCalculatePolynomialRegression((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 7: _t->requestUpdateOriginalWithAdjustedValues(); break;
        case 8: _t->requestSegmentFramesExport(); break;
        case 9: _t->showAvailableTags((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 10: _t->showAvailableAnchors((*reinterpret_cast< std::add_pointer_t<std::vector<int>>>(_a[1]))); break;
        case 11: _t->showPlotDistancesVsTimestamps((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2]))); break;
        case 12: _t->initDataAnalysis(); break;
        case 13: _t->startDataAnalysis(); break;
        case 14: _t->showPlotRollingDeviations((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[2]))); break;
        case 15: _t->validateRollingDeviationInput(); break;
        case 16: _t->validateThresholdInput(); break;
        case 17: _t->showDatasetSegments((*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[1]))); break;
        case 18: _t->showOriginalVsAdjustedDistances((*reinterpret_cast< std::add_pointer_t<std::vector<qlonglong>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<double*>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<std::vector<double>>>(_a[3]))); break;
        case 19: _t->uploadReferenceValues(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataAnalysisWindow::*)(const long long , const long long );
            if (_t _q_method = &DataAnalysisWindow::requestAnalyseData; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)(int );
            if (_t _q_method = &DataAnalysisWindow::requestCollectDataForTag; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)(const int );
            if (_t _q_method = &DataAnalysisWindow::requestCollectDataForAnchor; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)(const int );
            if (_t _q_method = &DataAnalysisWindow::requestCollectDataForPlotDistancesVsTimestamps; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)(const int );
            if (_t _q_method = &DataAnalysisWindow::requestCalculateRollingDeviation; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)(const double );
            if (_t _q_method = &DataAnalysisWindow::requestSplitDataset; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)(const std::vector<double> & );
            if (_t _q_method = &DataAnalysisWindow::requestCalculatePolynomialRegression; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)();
            if (_t _q_method = &DataAnalysisWindow::requestUpdateOriginalWithAdjustedValues; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (DataAnalysisWindow::*)();
            if (_t _q_method = &DataAnalysisWindow::requestSegmentFramesExport; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *DataAnalysisWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataAnalysisWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDataAnalysisWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DataAnalysisWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void DataAnalysisWindow::requestAnalyseData(const long long _t1, const long long _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataAnalysisWindow::requestCollectDataForTag(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataAnalysisWindow::requestCollectDataForAnchor(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataAnalysisWindow::requestCollectDataForPlotDistancesVsTimestamps(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataAnalysisWindow::requestCalculateRollingDeviation(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DataAnalysisWindow::requestSplitDataset(const double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DataAnalysisWindow::requestCalculatePolynomialRegression(const std::vector<double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DataAnalysisWindow::requestUpdateOriginalWithAdjustedValues()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void DataAnalysisWindow::requestSegmentFramesExport()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP

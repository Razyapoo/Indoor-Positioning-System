/****************************************************************************
** Meta object code from reading C++ file 'videoprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../IndoorPositioningSystem/videoprocessor.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoprocessor.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSVideoProcessorENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSVideoProcessorENDCLASS = QtMocHelpers::stringData(
    "VideoProcessor",
    "finished",
    "",
    "seekingDone",
    "processingIsPaused",
    "requestFindUWBMeasurementAndEnqueue",
    "position",
    "qImage",
    "DetectionData",
    "detectionData",
    "requestFindUWBMeasurementAndExport",
    "rangeIndex",
    "ExportType",
    "type",
    "lastRecord",
    "exportFinished",
    "success",
    "humanDetectorNotInitialized",
    "distCoeffLoaded",
    "processVideo",
    "cleanup"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSVideoProcessorENDCLASS_t {
    uint offsetsAndSizes[42];
    char stringdata0[15];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[19];
    char stringdata5[36];
    char stringdata6[9];
    char stringdata7[7];
    char stringdata8[14];
    char stringdata9[14];
    char stringdata10[35];
    char stringdata11[11];
    char stringdata12[11];
    char stringdata13[5];
    char stringdata14[11];
    char stringdata15[15];
    char stringdata16[8];
    char stringdata17[28];
    char stringdata18[16];
    char stringdata19[13];
    char stringdata20[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSVideoProcessorENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSVideoProcessorENDCLASS_t qt_meta_stringdata_CLASSVideoProcessorENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "VideoProcessor"
        QT_MOC_LITERAL(15, 8),  // "finished"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 11),  // "seekingDone"
        QT_MOC_LITERAL(37, 18),  // "processingIsPaused"
        QT_MOC_LITERAL(56, 35),  // "requestFindUWBMeasurementAndE..."
        QT_MOC_LITERAL(92, 8),  // "position"
        QT_MOC_LITERAL(101, 6),  // "qImage"
        QT_MOC_LITERAL(108, 13),  // "DetectionData"
        QT_MOC_LITERAL(122, 13),  // "detectionData"
        QT_MOC_LITERAL(136, 34),  // "requestFindUWBMeasurementAndE..."
        QT_MOC_LITERAL(171, 10),  // "rangeIndex"
        QT_MOC_LITERAL(182, 10),  // "ExportType"
        QT_MOC_LITERAL(193, 4),  // "type"
        QT_MOC_LITERAL(198, 10),  // "lastRecord"
        QT_MOC_LITERAL(209, 14),  // "exportFinished"
        QT_MOC_LITERAL(224, 7),  // "success"
        QT_MOC_LITERAL(232, 27),  // "humanDetectorNotInitialized"
        QT_MOC_LITERAL(260, 15),  // "distCoeffLoaded"
        QT_MOC_LITERAL(276, 12),  // "processVideo"
        QT_MOC_LITERAL(289, 7)   // "cleanup"
    },
    "VideoProcessor",
    "finished",
    "",
    "seekingDone",
    "processingIsPaused",
    "requestFindUWBMeasurementAndEnqueue",
    "position",
    "qImage",
    "DetectionData",
    "detectionData",
    "requestFindUWBMeasurementAndExport",
    "rangeIndex",
    "ExportType",
    "type",
    "lastRecord",
    "exportFinished",
    "success",
    "humanDetectorNotInitialized",
    "distCoeffLoaded",
    "processVideo",
    "cleanup"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSVideoProcessorENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x06,    1 /* Public */,
       3,    0,   75,    2, 0x06,    2 /* Public */,
       4,    0,   76,    2, 0x06,    3 /* Public */,
       5,    3,   77,    2, 0x06,    4 /* Public */,
      10,    5,   84,    2, 0x06,    8 /* Public */,
      15,    1,   95,    2, 0x06,   14 /* Public */,
      17,    0,   98,    2, 0x06,   16 /* Public */,
      18,    0,   99,    2, 0x06,   17 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      19,    0,  100,    2, 0x0a,   18 /* Public */,
      20,    0,  101,    2, 0x08,   19 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QImage, 0x80000000 | 8,    6,    7,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 12, 0x80000000 | 8, QMetaType::Bool,    6,   11,   13,    9,   14,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject VideoProcessor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSVideoProcessorENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSVideoProcessorENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSVideoProcessorENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<VideoProcessor, std::true_type>,
        // method 'finished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'seekingDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processingIsPaused'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'requestFindUWBMeasurementAndEnqueue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QImage, std::false_type>,
        QtPrivate::TypeAndForceComplete<DetectionData, std::false_type>,
        // method 'requestFindUWBMeasurementAndExport'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<ExportType, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DetectionData &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'exportFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'humanDetectorNotInitialized'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'distCoeffLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processVideo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cleanup'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void VideoProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoProcessor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->seekingDone(); break;
        case 2: _t->processingIsPaused(); break;
        case 3: _t->requestFindUWBMeasurementAndEnqueue((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QImage>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<DetectionData>>(_a[3]))); break;
        case 4: _t->requestFindUWBMeasurementAndExport((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ExportType>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<DetectionData>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[5]))); break;
        case 5: _t->exportFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->humanDetectorNotInitialized(); break;
        case 7: _t->distCoeffLoaded(); break;
        case 8: _t->processVideo(); break;
        case 9: _t->cleanup(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoProcessor::*)();
            if (_t _q_method = &VideoProcessor::finished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)();
            if (_t _q_method = &VideoProcessor::seekingDone; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)();
            if (_t _q_method = &VideoProcessor::processingIsPaused; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)(int , QImage , DetectionData );
            if (_t _q_method = &VideoProcessor::requestFindUWBMeasurementAndEnqueue; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)(int , int , ExportType , const DetectionData & , bool );
            if (_t _q_method = &VideoProcessor::requestFindUWBMeasurementAndExport; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)(bool );
            if (_t _q_method = &VideoProcessor::exportFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)();
            if (_t _q_method = &VideoProcessor::humanDetectorNotInitialized; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VideoProcessor::*)();
            if (_t _q_method = &VideoProcessor::distCoeffLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject *VideoProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSVideoProcessorENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void VideoProcessor::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VideoProcessor::seekingDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VideoProcessor::processingIsPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VideoProcessor::requestFindUWBMeasurementAndEnqueue(int _t1, QImage _t2, DetectionData _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VideoProcessor::requestFindUWBMeasurementAndExport(int _t1, int _t2, ExportType _t3, const DetectionData & _t4, bool _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VideoProcessor::exportFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void VideoProcessor::humanDetectorNotInitialized()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void VideoProcessor::distCoeffLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP

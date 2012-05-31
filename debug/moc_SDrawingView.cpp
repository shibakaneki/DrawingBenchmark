/****************************************************************************
** Meta object code from reading C++ file 'SDrawingView.h'
**
** Created: Thu May 31 10:22:31 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/SDrawingView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SDrawingView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SRubberBand[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SRubberBand[] = {
    "SRubberBand\0"
};

const QMetaObject SRubberBand::staticMetaObject = {
    { &QRubberBand::staticMetaObject, qt_meta_stringdata_SRubberBand,
      qt_meta_data_SRubberBand, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SRubberBand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SRubberBand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SRubberBand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SRubberBand))
        return static_cast<void*>(const_cast< SRubberBand*>(this));
    return QRubberBand::qt_metacast(_clname);
}

int SRubberBand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QRubberBand::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SDrawingView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   14,   13,   13, 0x05,
      45,   13,   13,   13, 0x05,
      77,   65,   13,   13, 0x05,
     136,  126,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     166,  153,   13,   13, 0x0a,
     191,   13,   13,   13, 0x0a,
     210,  205,   13,   13, 0x0a,
     234,   65,   13,   13, 0x0a,
     285,  283,   13,   13, 0x0a,
     315,  309,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SDrawingView[] = {
    "SDrawingView\0\0p\0currentPointChanged(QPointF)\0"
    "clearCoefficients()\0p0,p1,c0,c1\0"
    "addCoefficients(QPointF,QPointF,QPointF,QPointF)\0"
    "zoomDepth\0zoomChanged(int)\0smoothFactor\0"
    "onSmoothnessChanged(int)\0onClearPage()\0"
    "tool\0onSetCurrentTool(eTool)\0"
    "onPointSelected(QPointF,QPointF,QPointF,QPointF)\0"
    "w\0onLineWidthChanged(int)\0color\0"
    "onColorChanged(QColor)\0"
};

const QMetaObject SDrawingView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_SDrawingView,
      qt_meta_data_SDrawingView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SDrawingView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SDrawingView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SDrawingView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SDrawingView))
        return static_cast<void*>(const_cast< SDrawingView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int SDrawingView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentPointChanged((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: clearCoefficients(); break;
        case 2: addCoefficients((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< QPointF(*)>(_a[4]))); break;
        case 3: zoomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: onSmoothnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: onClearPage(); break;
        case 6: onSetCurrentTool((*reinterpret_cast< eTool(*)>(_a[1]))); break;
        case 7: onPointSelected((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< QPointF(*)>(_a[4]))); break;
        case 8: onLineWidthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: onColorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void SDrawingView::currentPointChanged(QPointF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SDrawingView::clearCoefficients()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SDrawingView::addCoefficients(QPointF _t1, QPointF _t2, QPointF _t3, QPointF _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SDrawingView::zoomChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

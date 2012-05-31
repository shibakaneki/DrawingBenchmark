/****************************************************************************
** Meta object code from reading C++ file 'SSettingsWidget.h'
**
** Created: Thu May 31 10:22:31 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/dockpalettes/SSettingsWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SSettingsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SSettingsWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   17,   16,   16, 0x05,
      63,   51,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     114,  110,   16,   16, 0x0a,
     136,   16,   16,   16, 0x0a,
     158,   51,   16,   16, 0x0a,
     213,  209,   16,   16, 0x08,
     243,  233,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SSettingsWidget[] = {
    "SSettingsWidget\0\0smoothness\0"
    "smoothnessChanged(int)\0p0,p1,c0,c1\0"
    "pointSelected(QPointF,QPointF,QPointF,QPointF)\0"
    "pos\0onPosChanged(QPointF)\0"
    "onClearCoefficients()\0"
    "onAddCoefficients(QPointF,QPointF,QPointF,QPointF)\0"
    "val\0onValueChanged(int)\0crnt,prev\0"
    "onPointSelected(QTreeWidgetItem*,QTreeWidgetItem*)\0"
};

const QMetaObject SSettingsWidget::staticMetaObject = {
    { &SDockPalette::staticMetaObject, qt_meta_stringdata_SSettingsWidget,
      qt_meta_data_SSettingsWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SSettingsWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SSettingsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SSettingsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SSettingsWidget))
        return static_cast<void*>(const_cast< SSettingsWidget*>(this));
    return SDockPalette::qt_metacast(_clname);
}

int SSettingsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SDockPalette::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: smoothnessChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: pointSelected((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< QPointF(*)>(_a[4]))); break;
        case 2: onPosChanged((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 3: onClearCoefficients(); break;
        case 4: onAddCoefficients((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2])),(*reinterpret_cast< QPointF(*)>(_a[3])),(*reinterpret_cast< QPointF(*)>(_a[4]))); break;
        case 5: onValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: onPointSelected((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SSettingsWidget::smoothnessChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SSettingsWidget::pointSelected(QPointF _t1, QPointF _t2, QPointF _t3, QPointF _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

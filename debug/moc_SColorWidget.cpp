/****************************************************************************
** Meta object code from reading C++ file 'SColorWidget.h'
**
** Created: Thu May 31 10:22:31 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/dockpalettes/SColorWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SColorWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SColorThumbnail[] = {

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

static const char qt_meta_stringdata_SColorThumbnail[] = {
    "SColorThumbnail\0"
};

const QMetaObject SColorThumbnail::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_SColorThumbnail,
      qt_meta_data_SColorThumbnail, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SColorThumbnail::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SColorThumbnail::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SColorThumbnail::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SColorThumbnail))
        return static_cast<void*>(const_cast< SColorThumbnail*>(this));
    return QLabel::qt_metacast(_clname);
}

int SColorThumbnail::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SColorPreview[] = {

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

static const char qt_meta_stringdata_SColorPreview[] = {
    "SColorPreview\0"
};

const QMetaObject SColorPreview::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SColorPreview,
      qt_meta_data_SColorPreview, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SColorPreview::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SColorPreview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SColorPreview::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SColorPreview))
        return static_cast<void*>(const_cast< SColorPreview*>(this));
    return QWidget::qt_metacast(_clname);
}

int SColorPreview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SColorPicker[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   38,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SColorPicker[] = {
    "SColorPicker\0\0colorComponentChanged()\0"
    "value\0onValueChanged(int)\0"
};

const QMetaObject SColorPicker::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SColorPicker,
      qt_meta_data_SColorPicker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SColorPicker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SColorPicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SColorPicker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SColorPicker))
        return static_cast<void*>(const_cast< SColorPicker*>(this));
    return QWidget::qt_metacast(_clname);
}

int SColorPicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: colorComponentChanged(); break;
        case 1: onValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SColorPicker::colorComponentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_SColorWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SColorWidget[] = {
    "SColorWidget\0\0color\0colorChanged(QColor)\0"
    "onColorComponentChanged()\0"
};

const QMetaObject SColorWidget::staticMetaObject = {
    { &SDockPalette::staticMetaObject, qt_meta_stringdata_SColorWidget,
      qt_meta_data_SColorWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SColorWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SColorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SColorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SColorWidget))
        return static_cast<void*>(const_cast< SColorWidget*>(this));
    return SDockPalette::qt_metacast(_clname);
}

int SColorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SDockPalette::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 1: onColorComponentChanged(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void SColorWidget::colorChanged(const QColor & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

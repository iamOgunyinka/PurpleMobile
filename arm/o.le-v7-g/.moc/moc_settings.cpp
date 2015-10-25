/****************************************************************************
** Meta object code from reading C++ file 'settings.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/settings.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Purple__Settings[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       6,   29, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,
      68,   47,   17,   17, 0x05,
     116,  102,   17,   17, 0x05,

 // properties: name, type, flags
     151,  143, 0x0a095001,
     158,  143, 0x0a095001,
     173,  169, 0x02095001,
     183,  143, 0x0a095103,
     192,  143, 0x0a495103,
     203,  143, 0x0a495103,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       2,
       1,

       0        // eod
};

static const char qt_meta_stringdata_Purple__Settings[] = {
    "Purple::Settings\0\0error\0errorOccurred(QString)\0"
    "newThumbnailsQuality\0"
    "thumbnailsQualityChanged(QString)\0"
    "newSafeSearch\0safeSearchChanged(QString)\0"
    "QString\0apiKey\0youtubeUrl\0int\0maxResult\0"
    "appTheme\0safeSearch\0thumbnailsQuality\0"
};

void Purple::Settings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Settings *_t = static_cast<Settings *>(_o);
        switch (_id) {
        case 0: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->thumbnailsQualityChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->safeSearchChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Purple::Settings::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Purple::Settings::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Purple__Settings,
      qt_meta_data_Purple__Settings, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Purple::Settings::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Purple::Settings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Purple::Settings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Purple__Settings))
        return static_cast<void*>(const_cast< Settings*>(this));
    return QObject::qt_metacast(_clname);
}

int Purple::Settings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = apiKey(); break;
        case 1: *reinterpret_cast< QString*>(_v) = youtubeUrl(); break;
        case 2: *reinterpret_cast< int*>(_v) = maxResult(); break;
        case 3: *reinterpret_cast< QString*>(_v) = appTheme(); break;
        case 4: *reinterpret_cast< QString*>(_v) = safeSearch(); break;
        case 5: *reinterpret_cast< QString*>(_v) = thumbnailsQuality(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: setAppTheme(*reinterpret_cast< QString*>(_v)); break;
        case 4: setSafeSearch(*reinterpret_cast< QString*>(_v)); break;
        case 5: setThumbnailsQuality(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Purple::Settings::errorOccurred(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Purple::Settings::thumbnailsQualityChanged(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Purple::Settings::safeSearchChanged(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

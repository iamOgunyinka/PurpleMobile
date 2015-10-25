/****************************************************************************
** Meta object code from reading C++ file 'SyncNetworkManager.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/SyncNetworkManager.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SyncNetworkManager.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Purple__SyncNetworkManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   28,   27,   27, 0x05,
      55,   28,   27,   27, 0x05,
      86,   78,   27,   27, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   27,   27,   27, 0x0a,
     117,   27,   27,   27, 0x0a,

 // methods: signature, parameters, type, tag, flags
     169,  161,   27,   27, 0x02,
     190,   27,   27,   27, 0x02,
     223,  218,  210,   27, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_Purple__SyncNetworkManager[] = {
    "Purple::SyncNetworkManager\0\0what\0"
    "networkError(QString)\0errorOccurred(QString)\0"
    "message\0finished(QString)\0onFinished()\0"
    "onNetworkError(QNetworkReply::NetworkError)\0"
    "request\0sendRequest(QString)\0"
    "cancelLastRequest()\0QString\0text\0"
    "toPercentageEncoding(QString)\0"
};

void Purple::SyncNetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SyncNetworkManager *_t = static_cast<SyncNetworkManager *>(_o);
        switch (_id) {
        case 0: _t->networkError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->finished((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onFinished(); break;
        case 4: _t->onNetworkError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 5: _t->sendRequest((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->cancelLastRequest(); break;
        case 7: { QString _r = _t->toPercentageEncoding((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Purple::SyncNetworkManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Purple::SyncNetworkManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Purple__SyncNetworkManager,
      qt_meta_data_Purple__SyncNetworkManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Purple::SyncNetworkManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Purple::SyncNetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Purple::SyncNetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Purple__SyncNetworkManager))
        return static_cast<void*>(const_cast< SyncNetworkManager*>(this));
    return QObject::qt_metacast(_clname);
}

int Purple::SyncNetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void Purple::SyncNetworkManager::networkError(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Purple::SyncNetworkManager::errorOccurred(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Purple::SyncNetworkManager::finished(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

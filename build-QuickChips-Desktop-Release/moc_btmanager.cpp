/****************************************************************************
** Meta object code from reading C++ file 'btmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QuickChips/IVDCore/btmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'btmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BTManager_t {
    QByteArrayData data[23];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BTManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BTManager_t qt_meta_stringdata_BTManager = {
    {
QT_MOC_LITERAL(0, 0, 9), // "BTManager"
QT_MOC_LITERAL(1, 10, 15), // "messageReceived"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "sender"
QT_MOC_LITERAL(4, 34, 7), // "message"
QT_MOC_LITERAL(5, 42, 15), // "clientConnected"
QT_MOC_LITERAL(6, 58, 4), // "name"
QT_MOC_LITERAL(7, 63, 18), // "clientDisconnected"
QT_MOC_LITERAL(8, 82, 11), // "serverError"
QT_MOC_LITERAL(9, 94, 23), // "QBluetoothServer::Error"
QT_MOC_LITERAL(10, 118, 5), // "error"
QT_MOC_LITERAL(11, 124, 10), // "readSocket"
QT_MOC_LITERAL(12, 135, 19), // "pairingConfirmation"
QT_MOC_LITERAL(13, 155, 12), // "confirmation"
QT_MOC_LITERAL(14, 168, 21), // "pairingDisplayPinCode"
QT_MOC_LITERAL(15, 190, 17), // "QBluetoothAddress"
QT_MOC_LITERAL(16, 208, 7), // "address"
QT_MOC_LITERAL(17, 216, 3), // "pin"
QT_MOC_LITERAL(18, 220, 26), // "pairingDisplayConfirmation"
QT_MOC_LITERAL(19, 247, 15), // "pairingFinished"
QT_MOC_LITERAL(20, 263, 30), // "QBluetoothLocalDevice::Pairing"
QT_MOC_LITERAL(21, 294, 7), // "pairing"
QT_MOC_LITERAL(22, 302, 28) // "QBluetoothLocalDevice::Error"

    },
    "BTManager\0messageReceived\0\0sender\0"
    "message\0clientConnected\0name\0"
    "clientDisconnected\0serverError\0"
    "QBluetoothServer::Error\0error\0readSocket\0"
    "pairingConfirmation\0confirmation\0"
    "pairingDisplayPinCode\0QBluetoothAddress\0"
    "address\0pin\0pairingDisplayConfirmation\0"
    "pairingFinished\0QBluetoothLocalDevice::Pairing\0"
    "pairing\0QBluetoothLocalDevice::Error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BTManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       5,    1,   79,    2, 0x06 /* Public */,
       7,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   85,    2, 0x08 /* Private */,
       5,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    1,   91,    2, 0x08 /* Private */,
      14,    2,   94,    2, 0x08 /* Private */,
      18,    2,   99,    2, 0x08 /* Private */,
      19,    2,  104,    2, 0x08 /* Private */,
      10,    1,  109,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, 0x80000000 | 15, QMetaType::QString,   16,   17,
    QMetaType::Void, 0x80000000 | 15, QMetaType::QString,   16,   17,
    QMetaType::Void, 0x80000000 | 15, 0x80000000 | 20,   16,   21,
    QMetaType::Void, 0x80000000 | 22,   10,

       0        // eod
};

void BTManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BTManager *_t = static_cast<BTManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->clientConnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->clientDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->serverError((*reinterpret_cast< QBluetoothServer::Error(*)>(_a[1]))); break;
        case 4: _t->clientConnected(); break;
        case 5: _t->clientDisconnected(); break;
        case 6: _t->readSocket(); break;
        case 7: _t->pairingConfirmation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->pairingDisplayPinCode((*reinterpret_cast< const QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->pairingDisplayConfirmation((*reinterpret_cast< const QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->pairingFinished((*reinterpret_cast< const QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QBluetoothLocalDevice::Pairing(*)>(_a[2]))); break;
        case 11: _t->error((*reinterpret_cast< QBluetoothLocalDevice::Error(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothLocalDevice::Pairing >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothLocalDevice::Error >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BTManager::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BTManager::messageReceived)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (BTManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BTManager::clientConnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (BTManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BTManager::clientDisconnected)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject BTManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BTManager.data,
      qt_meta_data_BTManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BTManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BTManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BTManager.stringdata0))
        return static_cast<void*>(const_cast< BTManager*>(this));
    return QObject::qt_metacast(_clname);
}

int BTManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void BTManager::messageReceived(const QString & _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BTManager::clientConnected(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BTManager::clientDisconnected(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

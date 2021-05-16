/****************************************************************************
** Meta object code from reading C++ file 'qtteardropwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QuickChips/qtteardropwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtteardropwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QTTeardropWindow_t {
    QByteArrayData data[5];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QTTeardropWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QTTeardropWindow_t qt_meta_stringdata_QTTeardropWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "QTTeardropWindow"
QT_MOC_LITERAL(1, 17, 19), // "menuItemWasSelected"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 8), // "itemName"
QT_MOC_LITERAL(4, 47, 16) // "leftArrowPressed"

    },
    "QTTeardropWindow\0menuItemWasSelected\0"
    "\0itemName\0leftArrowPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QTTeardropWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void QTTeardropWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QTTeardropWindow *_t = static_cast<QTTeardropWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->menuItemWasSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->leftArrowPressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QTTeardropWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTTeardropWindow::menuItemWasSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QTTeardropWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QTTeardropWindow::leftArrowPressed)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QTTeardropWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QTTeardropWindow.data,
      qt_meta_data_QTTeardropWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QTTeardropWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QTTeardropWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QTTeardropWindow.stringdata0))
        return static_cast<void*>(const_cast< QTTeardropWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int QTTeardropWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QTTeardropWindow::menuItemWasSelected(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QTTeardropWindow::leftArrowPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

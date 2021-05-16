/****************************************************************************
** Meta object code from reading C++ file 'quickchipsmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QuickChips/quickchipsmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'quickchipsmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QuickChipsMainWindow_t {
    QByteArrayData data[49];
    char stringdata0[889];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QuickChipsMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QuickChipsMainWindow_t qt_meta_stringdata_QuickChipsMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QuickChipsMainWindow"
QT_MOC_LITERAL(1, 21, 6), // "tapped"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 9), // "reconnect"
QT_MOC_LITERAL(4, 39, 11), // "closeCalled"
QT_MOC_LITERAL(5, 51, 13), // "roiWasClicked"
QT_MOC_LITERAL(6, 65, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 82, 8), // "treeItem"
QT_MOC_LITERAL(8, 91, 6), // "column"
QT_MOC_LITERAL(9, 98, 22), // "buttonAddRoiWasClicked"
QT_MOC_LITERAL(10, 121, 25), // "buttonAddSignalWasClicked"
QT_MOC_LITERAL(11, 147, 29), // "buttonAddBackgroundWasClicked"
QT_MOC_LITERAL(12, 177, 25), // "buttonDeleteRoiWasClicked"
QT_MOC_LITERAL(13, 203, 21), // "textBoxROINameChanged"
QT_MOC_LITERAL(14, 225, 18), // "textBoxROIXChanged"
QT_MOC_LITERAL(15, 244, 18), // "textBoxROIYChanged"
QT_MOC_LITERAL(16, 263, 23), // "textBoxROIRadiusChanged"
QT_MOC_LITERAL(17, 287, 19), // "menuItemWasSelected"
QT_MOC_LITERAL(18, 307, 8), // "itemName"
QT_MOC_LITERAL(19, 316, 14), // "receiveCommand"
QT_MOC_LITERAL(20, 331, 7), // "command"
QT_MOC_LITERAL(21, 339, 5), // "value"
QT_MOC_LITERAL(22, 345, 17), // "kl46StatusUpdated"
QT_MOC_LITERAL(23, 363, 11), // "updateFrame"
QT_MOC_LITERAL(24, 375, 17), // "btMessageReceived"
QT_MOC_LITERAL(25, 393, 6), // "sender"
QT_MOC_LITERAL(26, 400, 7), // "message"
QT_MOC_LITERAL(27, 408, 17), // "btClientConnected"
QT_MOC_LITERAL(28, 426, 4), // "name"
QT_MOC_LITERAL(29, 431, 20), // "btClientDisconnected"
QT_MOC_LITERAL(30, 452, 20), // "buttonCaptureClicked"
QT_MOC_LITERAL(31, 473, 24), // "buttonCaptureDarkClicked"
QT_MOC_LITERAL(32, 498, 22), // "buttonClearDarkClicked"
QT_MOC_LITERAL(33, 521, 17), // "buttonQuitClicked"
QT_MOC_LITERAL(34, 539, 22), // "buttonReconnectClicked"
QT_MOC_LITERAL(35, 562, 25), // "buttonStartProcessClicked"
QT_MOC_LITERAL(36, 588, 26), // "buttonReloadProcessClicked"
QT_MOC_LITERAL(37, 615, 16), // "buttonLEDClicked"
QT_MOC_LITERAL(38, 632, 19), // "buttonHeaterClicked"
QT_MOC_LITERAL(39, 652, 28), // "buttonSetHeaterTargetClicked"
QT_MOC_LITERAL(40, 681, 28), // "buttonGetHeaterTargetClicked"
QT_MOC_LITERAL(41, 710, 19), // "buttonDebugAClicked"
QT_MOC_LITERAL(42, 730, 19), // "buttonDebugBClicked"
QT_MOC_LITERAL(43, 750, 19), // "buttonBTSendClicked"
QT_MOC_LITERAL(44, 770, 31), // "getAndUpdateCameraExposureField"
QT_MOC_LITERAL(45, 802, 22), // "setExposureFromTextBox"
QT_MOC_LITERAL(46, 825, 25), // "updateCameraTickRateField"
QT_MOC_LITERAL(47, 851, 28), // "setCameraTickRateFromTextBox"
QT_MOC_LITERAL(48, 880, 8) // "debugLog"

    },
    "QuickChipsMainWindow\0tapped\0\0reconnect\0"
    "closeCalled\0roiWasClicked\0QTreeWidgetItem*\0"
    "treeItem\0column\0buttonAddRoiWasClicked\0"
    "buttonAddSignalWasClicked\0"
    "buttonAddBackgroundWasClicked\0"
    "buttonDeleteRoiWasClicked\0"
    "textBoxROINameChanged\0textBoxROIXChanged\0"
    "textBoxROIYChanged\0textBoxROIRadiusChanged\0"
    "menuItemWasSelected\0itemName\0"
    "receiveCommand\0command\0value\0"
    "kl46StatusUpdated\0updateFrame\0"
    "btMessageReceived\0sender\0message\0"
    "btClientConnected\0name\0btClientDisconnected\0"
    "buttonCaptureClicked\0buttonCaptureDarkClicked\0"
    "buttonClearDarkClicked\0buttonQuitClicked\0"
    "buttonReconnectClicked\0buttonStartProcessClicked\0"
    "buttonReloadProcessClicked\0buttonLEDClicked\0"
    "buttonHeaterClicked\0buttonSetHeaterTargetClicked\0"
    "buttonGetHeaterTargetClicked\0"
    "buttonDebugAClicked\0buttonDebugBClicked\0"
    "buttonBTSendClicked\0getAndUpdateCameraExposureField\0"
    "setExposureFromTextBox\0updateCameraTickRateField\0"
    "setCameraTickRateFromTextBox\0debugLog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QuickChipsMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  204,    2, 0x06 /* Public */,
       3,    0,  205,    2, 0x06 /* Public */,
       4,    0,  206,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,  207,    2, 0x08 /* Private */,
       9,    0,  212,    2, 0x08 /* Private */,
      10,    0,  213,    2, 0x08 /* Private */,
      11,    0,  214,    2, 0x08 /* Private */,
      12,    0,  215,    2, 0x08 /* Private */,
      13,    0,  216,    2, 0x08 /* Private */,
      14,    0,  217,    2, 0x08 /* Private */,
      15,    0,  218,    2, 0x08 /* Private */,
      16,    0,  219,    2, 0x08 /* Private */,
      17,    1,  220,    2, 0x0a /* Public */,
      19,    2,  223,    2, 0x0a /* Public */,
      22,    0,  228,    2, 0x08 /* Private */,
      23,    0,  229,    2, 0x08 /* Private */,
      24,    2,  230,    2, 0x08 /* Private */,
      27,    1,  235,    2, 0x08 /* Private */,
      29,    1,  238,    2, 0x08 /* Private */,
      30,    0,  241,    2, 0x08 /* Private */,
      31,    0,  242,    2, 0x08 /* Private */,
      32,    0,  243,    2, 0x08 /* Private */,
      33,    0,  244,    2, 0x08 /* Private */,
      34,    0,  245,    2, 0x08 /* Private */,
      35,    0,  246,    2, 0x08 /* Private */,
      36,    0,  247,    2, 0x08 /* Private */,
      37,    0,  248,    2, 0x08 /* Private */,
      38,    0,  249,    2, 0x08 /* Private */,
      39,    0,  250,    2, 0x08 /* Private */,
      40,    0,  251,    2, 0x08 /* Private */,
      41,    0,  252,    2, 0x08 /* Private */,
      42,    0,  253,    2, 0x08 /* Private */,
      43,    0,  254,    2, 0x08 /* Private */,
      44,    0,  255,    2, 0x08 /* Private */,
      45,    0,  256,    2, 0x08 /* Private */,
      46,    0,  257,    2, 0x08 /* Private */,
      47,    0,  258,    2, 0x08 /* Private */,
      48,    1,  259,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   20,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   25,   26,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void, QMetaType::QString,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,

       0        // eod
};

void QuickChipsMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QuickChipsMainWindow *_t = static_cast<QuickChipsMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tapped(); break;
        case 1: _t->reconnect(); break;
        case 2: _t->closeCalled(); break;
        case 3: _t->roiWasClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->buttonAddRoiWasClicked(); break;
        case 5: _t->buttonAddSignalWasClicked(); break;
        case 6: _t->buttonAddBackgroundWasClicked(); break;
        case 7: _t->buttonDeleteRoiWasClicked(); break;
        case 8: _t->textBoxROINameChanged(); break;
        case 9: _t->textBoxROIXChanged(); break;
        case 10: _t->textBoxROIYChanged(); break;
        case 11: _t->textBoxROIRadiusChanged(); break;
        case 12: _t->menuItemWasSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->receiveCommand((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->kl46StatusUpdated(); break;
        case 15: _t->updateFrame(); break;
        case 16: _t->btMessageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 17: _t->btClientConnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->btClientDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->buttonCaptureClicked(); break;
        case 20: _t->buttonCaptureDarkClicked(); break;
        case 21: _t->buttonClearDarkClicked(); break;
        case 22: _t->buttonQuitClicked(); break;
        case 23: _t->buttonReconnectClicked(); break;
        case 24: _t->buttonStartProcessClicked(); break;
        case 25: _t->buttonReloadProcessClicked(); break;
        case 26: _t->buttonLEDClicked(); break;
        case 27: _t->buttonHeaterClicked(); break;
        case 28: _t->buttonSetHeaterTargetClicked(); break;
        case 29: _t->buttonGetHeaterTargetClicked(); break;
        case 30: _t->buttonDebugAClicked(); break;
        case 31: _t->buttonDebugBClicked(); break;
        case 32: _t->buttonBTSendClicked(); break;
        case 33: _t->getAndUpdateCameraExposureField(); break;
        case 34: _t->setExposureFromTextBox(); break;
        case 35: _t->updateCameraTickRateField(); break;
        case 36: _t->setCameraTickRateFromTextBox(); break;
        case 37: _t->debugLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QuickChipsMainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QuickChipsMainWindow::tapped)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QuickChipsMainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QuickChipsMainWindow::reconnect)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QuickChipsMainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QuickChipsMainWindow::closeCalled)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QuickChipsMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QuickChipsMainWindow.data,
      qt_meta_data_QuickChipsMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QuickChipsMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QuickChipsMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QuickChipsMainWindow.stringdata0))
        return static_cast<void*>(const_cast< QuickChipsMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QuickChipsMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void QuickChipsMainWindow::tapped()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QuickChipsMainWindow::reconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void QuickChipsMainWindow::closeCalled()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

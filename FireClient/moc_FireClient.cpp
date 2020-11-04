/****************************************************************************
** Meta object code from reading C++ file 'FireClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FireClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FireClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FireClient_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FireClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FireClient_t qt_meta_stringdata_FireClient = {
    {
QT_MOC_LITERAL(0, 0, 10) // "FireClient"

    },
    "FireClient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FireClient[] = {

 // content:
       8,       // revision
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

void FireClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject FireClient::staticMetaObject = { {
    &QMqttClient::staticMetaObject,
    qt_meta_stringdata_FireClient.data,
    qt_meta_data_FireClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FireClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FireClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FireClient.stringdata0))
        return static_cast<void*>(this);
    return QMqttClient::qt_metacast(_clname);
}

int FireClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMqttClient::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_FireForm_t {
    QByteArrayData data[21];
    char stringdata0[377];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FireForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FireForm_t qt_meta_stringdata_FireForm = {
    {
QT_MOC_LITERAL(0, 0, 8), // "FireForm"
QT_MOC_LITERAL(1, 9, 15), // "serialopenSinal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 13), // "publishSignal"
QT_MOC_LITERAL(4, 40, 12), // "QMqttClient*"
QT_MOC_LITERAL(5, 53, 6), // "client"
QT_MOC_LITERAL(6, 60, 5), // "topic"
QT_MOC_LITERAL(7, 66, 7), // "message"
QT_MOC_LITERAL(8, 74, 15), // "subscribeSignal"
QT_MOC_LITERAL(9, 90, 17), // "unsubscribeSignal"
QT_MOC_LITERAL(10, 108, 13), // "connectSignal"
QT_MOC_LITERAL(11, 122, 24), // "on_buttonPublish_clicked"
QT_MOC_LITERAL(12, 147, 24), // "on_buttonConnect_clicked"
QT_MOC_LITERAL(13, 172, 26), // "on_buttonSubscribe_clicked"
QT_MOC_LITERAL(14, 199, 28), // "on_buttonUnsubscribe_clicked"
QT_MOC_LITERAL(15, 228, 27), // "on_editLog_selectionChanged"
QT_MOC_LITERAL(16, 256, 22), // "on_radioButton_clicked"
QT_MOC_LITERAL(17, 279, 24), // "on_radioButton_2_clicked"
QT_MOC_LITERAL(18, 304, 24), // "on_radioButton_3_clicked"
QT_MOC_LITERAL(19, 329, 24), // "on_radioButton_4_clicked"
QT_MOC_LITERAL(20, 354, 22) // "on_buttonClear_clicked"

    },
    "FireForm\0serialopenSinal\0\0publishSignal\0"
    "QMqttClient*\0client\0topic\0message\0"
    "subscribeSignal\0unsubscribeSignal\0"
    "connectSignal\0on_buttonPublish_clicked\0"
    "on_buttonConnect_clicked\0"
    "on_buttonSubscribe_clicked\0"
    "on_buttonUnsubscribe_clicked\0"
    "on_editLog_selectionChanged\0"
    "on_radioButton_clicked\0on_radioButton_2_clicked\0"
    "on_radioButton_3_clicked\0"
    "on_radioButton_4_clicked\0"
    "on_buttonClear_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FireForm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       1,    0,   97,    2, 0x26 /* Public | MethodCloned */,
       3,    3,   98,    2, 0x06 /* Public */,
       8,    0,  105,    2, 0x06 /* Public */,
       9,    0,  106,    2, 0x06 /* Public */,
      10,    1,  107,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  110,    2, 0x08 /* Private */,
      12,    0,  111,    2, 0x08 /* Private */,
      13,    0,  112,    2, 0x08 /* Private */,
      14,    0,  113,    2, 0x08 /* Private */,
      15,    0,  114,    2, 0x08 /* Private */,
      16,    0,  115,    2, 0x08 /* Private */,
      17,    0,  116,    2, 0x08 /* Private */,
      18,    0,  117,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::QString, QMetaType::QByteArray,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
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

       0        // eod
};

void FireForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FireForm *_t = static_cast<FireForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serialopenSinal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->serialopenSinal(); break;
        case 2: _t->publishSignal((*reinterpret_cast< QMqttClient*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 3: _t->subscribeSignal(); break;
        case 4: _t->unsubscribeSignal(); break;
        case 5: _t->connectSignal((*reinterpret_cast< QMqttClient*(*)>(_a[1]))); break;
        case 6: _t->on_buttonPublish_clicked(); break;
        case 7: _t->on_buttonConnect_clicked(); break;
        case 8: _t->on_buttonSubscribe_clicked(); break;
        case 9: _t->on_buttonUnsubscribe_clicked(); break;
        case 10: _t->on_editLog_selectionChanged(); break;
        case 11: _t->on_radioButton_clicked(); break;
        case 12: _t->on_radioButton_2_clicked(); break;
        case 13: _t->on_radioButton_3_clicked(); break;
        case 14: _t->on_radioButton_4_clicked(); break;
        case 15: _t->on_buttonClear_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FireForm::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FireForm::serialopenSinal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FireForm::*)(QMqttClient * , const QString & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FireForm::publishSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FireForm::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FireForm::subscribeSignal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FireForm::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FireForm::unsubscribeSignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FireForm::*)(QMqttClient * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FireForm::connectSignal)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FireForm::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_FireForm.data,
    qt_meta_data_FireForm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FireForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FireForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FireForm.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FireForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void FireForm::serialopenSinal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void FireForm::publishSignal(QMqttClient * _t1, const QString & _t2, const QByteArray & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FireForm::subscribeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void FireForm::unsubscribeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void FireForm::connectSignal(QMqttClient * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

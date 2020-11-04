/****************************************************************************
** Meta object code from reading C++ file 'gsvr_app_config.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gsvr_app_config.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gsvr_app_config.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConfigWin_t {
    QByteArrayData data[25];
    char stringdata0[487];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConfigWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConfigWin_t qt_meta_stringdata_ConfigWin = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ConfigWin"
QT_MOC_LITERAL(1, 10, 15), // "serialopenSinal"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "publishSignal"
QT_MOC_LITERAL(4, 41, 5), // "topic"
QT_MOC_LITERAL(5, 47, 7), // "message"
QT_MOC_LITERAL(6, 55, 15), // "subscribeSignal"
QT_MOC_LITERAL(7, 71, 17), // "unsubscribeSignal"
QT_MOC_LITERAL(8, 89, 13), // "connectSignal"
QT_MOC_LITERAL(9, 103, 12), // "QMqttClient*"
QT_MOC_LITERAL(10, 116, 6), // "client"
QT_MOC_LITERAL(11, 123, 29), // "on_pushButton_comopen_clicked"
QT_MOC_LITERAL(12, 153, 9), // "readMyCom"
QT_MOC_LITERAL(13, 163, 31), // "on_pushButton_emi_write_clicked"
QT_MOC_LITERAL(14, 195, 18), // "DeleteCurrentIndex"
QT_MOC_LITERAL(15, 214, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(16, 236, 29), // "on_pushButton_sendNum_clicked"
QT_MOC_LITERAL(17, 266, 29), // "on_pushButton_recvNum_clicked"
QT_MOC_LITERAL(18, 296, 24), // "on_buttonPublish_clicked"
QT_MOC_LITERAL(19, 321, 24), // "on_buttonConnect_clicked"
QT_MOC_LITERAL(20, 346, 26), // "on_buttonSubscribe_clicked"
QT_MOC_LITERAL(21, 373, 28), // "on_buttonUnsubscribe_clicked"
QT_MOC_LITERAL(22, 402, 30), // "on_buttonConnect_local_clicked"
QT_MOC_LITERAL(23, 433, 31), // "on_pushButton_emi_write_pressed"
QT_MOC_LITERAL(24, 465, 21) // "on_buttonSave_clicked"

    },
    "ConfigWin\0serialopenSinal\0\0publishSignal\0"
    "topic\0message\0subscribeSignal\0"
    "unsubscribeSignal\0connectSignal\0"
    "QMqttClient*\0client\0on_pushButton_comopen_clicked\0"
    "readMyCom\0on_pushButton_emi_write_clicked\0"
    "DeleteCurrentIndex\0on_pushButton_clicked\0"
    "on_pushButton_sendNum_clicked\0"
    "on_pushButton_recvNum_clicked\0"
    "on_buttonPublish_clicked\0"
    "on_buttonConnect_clicked\0"
    "on_buttonSubscribe_clicked\0"
    "on_buttonUnsubscribe_clicked\0"
    "on_buttonConnect_local_clicked\0"
    "on_pushButton_emi_write_pressed\0"
    "on_buttonSave_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConfigWin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  114,    2, 0x06 /* Public */,
       1,    0,  117,    2, 0x26 /* Public | MethodCloned */,
       3,    2,  118,    2, 0x06 /* Public */,
       6,    0,  123,    2, 0x06 /* Public */,
       7,    0,  124,    2, 0x06 /* Public */,
       8,    1,  125,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  128,    2, 0x08 /* Private */,
      12,    0,  129,    2, 0x08 /* Private */,
      13,    0,  130,    2, 0x08 /* Private */,
      14,    0,  131,    2, 0x08 /* Private */,
      15,    0,  132,    2, 0x08 /* Private */,
      16,    0,  133,    2, 0x08 /* Private */,
      17,    0,  134,    2, 0x08 /* Private */,
      18,    0,  135,    2, 0x08 /* Private */,
      19,    0,  136,    2, 0x08 /* Private */,
      20,    0,  137,    2, 0x08 /* Private */,
      21,    0,  138,    2, 0x08 /* Private */,
      22,    0,  139,    2, 0x08 /* Private */,
      23,    0,  140,    2, 0x08 /* Private */,
      24,    0,  141,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConfigWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConfigWin *_t = static_cast<ConfigWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->serialopenSinal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->serialopenSinal(); break;
        case 2: _t->publishSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->subscribeSignal(); break;
        case 4: _t->unsubscribeSignal(); break;
        case 5: _t->connectSignal((*reinterpret_cast< QMqttClient*(*)>(_a[1]))); break;
        case 6: _t->on_pushButton_comopen_clicked(); break;
        case 7: _t->readMyCom(); break;
        case 8: _t->on_pushButton_emi_write_clicked(); break;
        case 9: _t->DeleteCurrentIndex(); break;
        case 10: _t->on_pushButton_clicked(); break;
        case 11: _t->on_pushButton_sendNum_clicked(); break;
        case 12: _t->on_pushButton_recvNum_clicked(); break;
        case 13: _t->on_buttonPublish_clicked(); break;
        case 14: _t->on_buttonConnect_clicked(); break;
        case 15: _t->on_buttonSubscribe_clicked(); break;
        case 16: _t->on_buttonUnsubscribe_clicked(); break;
        case 17: _t->on_buttonConnect_local_clicked(); break;
        case 18: _t->on_pushButton_emi_write_pressed(); break;
        case 19: _t->on_buttonSave_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
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
            using _t = void (ConfigWin::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigWin::serialopenSinal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConfigWin::*)(const QString & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigWin::publishSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ConfigWin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigWin::subscribeSignal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ConfigWin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigWin::unsubscribeSignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ConfigWin::*)(QMqttClient * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConfigWin::connectSignal)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConfigWin::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ConfigWin.data,
    qt_meta_data_ConfigWin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConfigWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigWin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigWin.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "MqttPublic"))
        return static_cast< MqttPublic*>(this);
    return QDialog::qt_metacast(_clname);
}

int ConfigWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void ConfigWin::serialopenSinal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void ConfigWin::publishSignal(const QString & _t1, const QByteArray & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ConfigWin::subscribeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ConfigWin::unsubscribeSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ConfigWin::connectSignal(QMqttClient * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

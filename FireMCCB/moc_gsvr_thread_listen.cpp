/****************************************************************************
** Meta object code from reading C++ file 'gsvr_thread_listen.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gsvr_thread_listen.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gsvr_thread_listen.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ThreadMqttListen_t {
    QByteArrayData data[9];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThreadMqttListen_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThreadMqttListen_t qt_meta_stringdata_ThreadMqttListen = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ThreadMqttListen"
QT_MOC_LITERAL(1, 17, 18), // "mesgResponseSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 8), // "uint32_t"
QT_MOC_LITERAL(4, 46, 7), // "seq_Num"
QT_MOC_LITERAL(5, 54, 5), // "topic"
QT_MOC_LITERAL(6, 60, 7), // "message"
QT_MOC_LITERAL(7, 68, 16), // "askGetFileSignal"
QT_MOC_LITERAL(8, 85, 6) // "Myslot"

    },
    "ThreadMqttListen\0mesgResponseSignal\0"
    "\0uint32_t\0seq_Num\0topic\0message\0"
    "askGetFileSignal\0Myslot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThreadMqttListen[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,
       7,    0,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ThreadMqttListen::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThreadMqttListen *_t = static_cast<ThreadMqttListen *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mesgResponseSignal((*reinterpret_cast< uint32_t(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->askGetFileSignal(); break;
        case 2: _t->Myslot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ThreadMqttListen::*)(uint32_t , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreadMqttListen::mesgResponseSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ThreadMqttListen::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreadMqttListen::askGetFileSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ThreadMqttListen::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ThreadMqttListen.data,
    qt_meta_data_ThreadMqttListen,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ThreadMqttListen::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThreadMqttListen::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ThreadMqttListen.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "MqttPublic"))
        return static_cast< MqttPublic*>(this);
    return QObject::qt_metacast(_clname);
}

int ThreadMqttListen::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ThreadMqttListen::mesgResponseSignal(uint32_t _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ThreadMqttListen::askGetFileSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

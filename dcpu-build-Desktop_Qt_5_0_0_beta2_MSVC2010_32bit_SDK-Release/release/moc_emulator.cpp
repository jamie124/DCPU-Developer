/****************************************************************************
** Meta object code from reading C++ file 'emulator.h'
**
** Created: Wed Nov 28 20:21:50 2012
**      by: The Qt Meta Object Compiler version 67 (Qt 5.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/emulator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Emulator_t {
    QByteArrayData data[7];
    char stringdata[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Emulator_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Emulator_t qt_meta_stringdata_Emulator = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 14),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 12),
QT_MOC_LITERAL(4, 38, 16),
QT_MOC_LITERAL(5, 55, 13),
QT_MOC_LITERAL(6, 69, 14)
    },
    "Emulator\0fullMemorySync\0\0memory_array\0"
    "registersChanged\0registers_ptr\0"
    "emulationEnded\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Emulator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x05,
       4,    1,   32,    2, 0x05,
       6,    1,   35,    2, 0x05,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Emulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Emulator *_t = static_cast<Emulator *>(_o);
        switch (_id) {
        case 0: _t->fullMemorySync((*reinterpret_cast< memory_array(*)>(_a[1]))); break;
        case 1: _t->registersChanged((*reinterpret_cast< registers_ptr(*)>(_a[1]))); break;
        case 2: _t->emulationEnded((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Emulator::*_t)(memory_array );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Emulator::fullMemorySync)) {
                *result = 0;
            }
        }
        {
            typedef void (Emulator::*_t)(registers_ptr );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Emulator::registersChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (Emulator::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Emulator::emulationEnded)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Emulator::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Emulator.data,
      qt_meta_data_Emulator,  qt_static_metacall, 0, 0}
};


const QMetaObject *Emulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Emulator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Emulator.stringdata))
        return static_cast<void*>(const_cast< Emulator*>(this));
    return QThread::qt_metacast(_clname);
}

int Emulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void Emulator::fullMemorySync(memory_array _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Emulator::registersChanged(registers_ptr _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Emulator::emulationEnded(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

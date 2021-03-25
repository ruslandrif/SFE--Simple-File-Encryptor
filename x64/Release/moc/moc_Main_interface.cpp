/****************************************************************************
** Meta object code from reading C++ file 'Main_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Main_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Main_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Main_interface_t {
    QByteArrayData data[10];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Main_interface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Main_interface_t qt_meta_stringdata_Main_interface = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Main_interface"
QT_MOC_LITERAL(1, 15, 4), // "menu"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 13), // "show_settings"
QT_MOC_LITERAL(4, 35, 13), // "start_program"
QT_MOC_LITERAL(5, 49, 15), // "search_for_file"
QT_MOC_LITERAL(6, 65, 5), // "WHICH"
QT_MOC_LITERAL(7, 71, 23), // "ready_to_encrypt_screen"
QT_MOC_LITERAL(8, 95, 12), // "final_screen"
QT_MOC_LITERAL(9, 108, 18) // "encryptor_finished"

    },
    "Main_interface\0menu\0\0show_settings\0"
    "start_program\0search_for_file\0WHICH\0"
    "ready_to_encrypt_screen\0final_screen\0"
    "encryptor_finished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Main_interface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    1,   52,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x0a /* Public */,
       8,    0,   56,    2, 0x0a /* Public */,
       9,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Main_interface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Main_interface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->menu(); break;
        case 1: _t->show_settings(); break;
        case 2: _t->start_program(); break;
        case 3: _t->search_for_file((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 4: _t->ready_to_encrypt_screen(); break;
        case 5: _t->final_screen(); break;
        case 6: _t->encryptor_finished(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Main_interface::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Main_interface.data,
    qt_meta_data_Main_interface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Main_interface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Main_interface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Main_interface.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Main_interface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

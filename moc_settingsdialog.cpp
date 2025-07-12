/****************************************************************************
** Meta object code from reading C++ file 'settingsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "settingsdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingsDialog_t {
    QByteArrayData data[13];
    char stringdata0[284];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsDialog_t qt_meta_stringdata_SettingsDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SettingsDialog"
QT_MOC_LITERAL(1, 15, 21), // "on_buttonBox_rejected"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 21), // "on_buttonBox_accepted"
QT_MOC_LITERAL(4, 60, 23), // "on_btnDirBrowse_clicked"
QT_MOC_LITERAL(5, 84, 27), // "on_pb_fontDlgActive_clicked"
QT_MOC_LITERAL(6, 112, 29), // "on_pb_fontDlgInactive_clicked"
QT_MOC_LITERAL(7, 142, 28), // "on_pb_colorDlgActive_clicked"
QT_MOC_LITERAL(8, 171, 30), // "on_pb_colorDlgInactive_clicked"
QT_MOC_LITERAL(9, 202, 25), // "on_cb_hotKey_stateChanged"
QT_MOC_LITERAL(10, 228, 4), // "arg1"
QT_MOC_LITERAL(11, 233, 26), // "on_pb_warningColor_clicked"
QT_MOC_LITERAL(12, 260, 23) // "on_pb_lateColor_clicked"

    },
    "SettingsDialog\0on_buttonBox_rejected\0"
    "\0on_buttonBox_accepted\0on_btnDirBrowse_clicked\0"
    "on_pb_fontDlgActive_clicked\0"
    "on_pb_fontDlgInactive_clicked\0"
    "on_pb_colorDlgActive_clicked\0"
    "on_pb_colorDlgInactive_clicked\0"
    "on_cb_hotKey_stateChanged\0arg1\0"
    "on_pb_warningColor_clicked\0"
    "on_pb_lateColor_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    1,   71,    2, 0x08 /* Private */,
      11,    0,   74,    2, 0x08 /* Private */,
      12,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SettingsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_buttonBox_rejected(); break;
        case 1: _t->on_buttonBox_accepted(); break;
        case 2: _t->on_btnDirBrowse_clicked(); break;
        case 3: _t->on_pb_fontDlgActive_clicked(); break;
        case 4: _t->on_pb_fontDlgInactive_clicked(); break;
        case 5: _t->on_pb_colorDlgActive_clicked(); break;
        case 6: _t->on_pb_colorDlgInactive_clicked(); break;
        case 7: _t->on_cb_hotKey_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_pb_warningColor_clicked(); break;
        case 9: _t->on_pb_lateColor_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SettingsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SettingsDialog.data,
    qt_meta_data_SettingsDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

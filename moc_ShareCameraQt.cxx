/****************************************************************************
** Meta object code from reading C++ file 'ShareCameraQt.h'
**
** Created: Sun Apr 21 14:10:37 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ShareCameraQt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ShareCameraQt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ShareCameraQt[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      34,   26,   14,   14, 0x08,
      62,   14,   14,   14, 0x08,
      75,   14,   14,   14, 0x08,
      89,   14,   14,   14, 0x08,
     100,   26,   14,   14, 0x08,
     129,   14,   14,   14, 0x08,
     159,   26,   14,   14, 0x08,
     192,  187,   14,   14, 0x08,
     233,   14,   14,   14, 0x08,
     257,   14,   14,   14, 0x08,
     283,   14,   14,   14, 0x08,
     309,   14,   14,   14, 0x08,
     335,   14,   14,   14, 0x08,
     361,   14,   14,   14, 0x08,
     393,  387,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ShareCameraQt[] = {
    "ShareCameraQt\0\0slotExit()\0checked\0"
    "on_checkBox_9_toggled(bool)\0flipMeshes()\0"
    "writeMeshes()\0openVTKS()\0"
    "on_checkBox_10_toggled(bool)\0"
    "on_lineEdit_editingFinished()\0"
    "on_checkBox_3_toggled(bool)\0arg1\0"
    "on_comboBox_currentIndexChanged(QString)\0"
    "on_toolButton_clicked()\0"
    "on_toolButton_2_clicked()\0"
    "on_toolButton_3_clicked()\0"
    "on_toolButton_4_clicked()\0"
    "on_toolButton_5_clicked()\0"
    "on_toolButton_6_clicked()\0x,y,z\0"
    "viewChange(int,int,int)\0"
};

void ShareCameraQt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ShareCameraQt *_t = static_cast<ShareCameraQt *>(_o);
        switch (_id) {
        case 0: _t->slotExit(); break;
        case 1: _t->on_checkBox_9_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->flipMeshes(); break;
        case 3: _t->writeMeshes(); break;
        case 4: _t->openVTKS(); break;
        case 5: _t->on_checkBox_10_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_lineEdit_editingFinished(); break;
        case 7: _t->on_checkBox_3_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->on_toolButton_clicked(); break;
        case 10: _t->on_toolButton_2_clicked(); break;
        case 11: _t->on_toolButton_3_clicked(); break;
        case 12: _t->on_toolButton_4_clicked(); break;
        case 13: _t->on_toolButton_5_clicked(); break;
        case 14: _t->on_toolButton_6_clicked(); break;
        case 15: _t->viewChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ShareCameraQt::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ShareCameraQt::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ShareCameraQt,
      qt_meta_data_ShareCameraQt, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ShareCameraQt::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ShareCameraQt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ShareCameraQt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ShareCameraQt))
        return static_cast<void*>(const_cast< ShareCameraQt*>(this));
    if (!strcmp(_clname, "Ui::ShareCameraQt"))
        return static_cast< Ui::ShareCameraQt*>(const_cast< ShareCameraQt*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ShareCameraQt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

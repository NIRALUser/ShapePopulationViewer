/****************************************************************************
** Meta object code from reading C++ file 'ShapePopulationViewer.h'
**
** Created: Wed May 1 22:06:10 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ShapePopulationViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ShapePopulationViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ShapePopulationViewer[] = {

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
      23,   22,   22,   22, 0x0a,
      42,   34,   22,   22, 0x09,
      70,   22,   22,   22, 0x09,
      83,   22,   22,   22, 0x09,
      97,   22,   22,   22, 0x09,
     108,   34,   22,   22, 0x09,
     137,   22,   22,   22, 0x09,
     167,   34,   22,   22, 0x09,
     200,  195,   22,   22, 0x09,
     241,   22,   22,   22, 0x09,
     265,   22,   22,   22, 0x09,
     291,   22,   22,   22, 0x09,
     317,   22,   22,   22, 0x09,
     343,   22,   22,   22, 0x09,
     369,   22,   22,   22, 0x09,
     401,  395,   22,   22, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_ShapePopulationViewer[] = {
    "ShapePopulationViewer\0\0slotExit()\0"
    "checked\0on_checkBox_9_toggled(bool)\0"
    "flipMeshes()\0writeMeshes()\0openVTKS()\0"
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

void ShapePopulationViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ShapePopulationViewer *_t = static_cast<ShapePopulationViewer *>(_o);
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

const QMetaObjectExtraData ShapePopulationViewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ShapePopulationViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ShapePopulationViewer,
      qt_meta_data_ShapePopulationViewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ShapePopulationViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ShapePopulationViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ShapePopulationViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ShapePopulationViewer))
        return static_cast<void*>(const_cast< ShapePopulationViewer*>(this));
    if (!strcmp(_clname, "Ui::ShapePopulationViewer"))
        return static_cast< Ui::ShapePopulationViewer*>(const_cast< ShapePopulationViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ShapePopulationViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

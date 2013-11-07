#ifndef SHAPEPOPULATIONQT_H
#define SHAPEPOPULATIONQT_H

// local
#include "ui_ShapePopulationQT.h"
#include "ShapePopulationBase.h"
#include "gradientWidgetQT.h"
#include "cameraDialogQT.h"
#include "backgroundDialogQT.h"
#include "CSVloaderQT.h"
#include <iostream>

// QT
#include <QMainWindow>
#include <QVTKWidget.h>
#include <QFileDialog>              //Open directory/files
#include <QFileInfo>                //Use Files
#include <QKeyEvent>                //KeyPressEvent
#include <QStandardItemModel>       //Data Array
#include <QColorDialog>             //ColorPicker
#include <vtkDelimitedTextReader.h> //CSVloader
#include <QUrl>                     //DropFiles


#include <QDebug>



class ShapePopulationQT : public QMainWindow, public Ui::ShapePopulationQT, public ShapePopulationBase
{
  Q_OBJECT

  public:

    ShapePopulationQT();
    ~ShapePopulationQT();

    void loadVTKFileCLP(QFileInfo file);
    void loadCSVFileCLP(QFileInfo file);
    void loadVTKDirCLP(QDir vtkDir);
    void loadColorMapCLP(std::string a_filePath);
    void loadCameraCLP(std::string a_filePath);


  protected:

    bool m_toolsDisplayed;
    unsigned int m_numberOfMeshes;
    QString m_lastDirectory;
    QString m_colormapDirectory;
    QFileInfoList m_fileList;
    std::vector<QVTKWidget *> m_widgetList;
    cameraDialogQT * m_cameraDialog;
    backgroundDialogQT * m_backgroundDialog;
    CSVloaderQT * m_CSVloaderDialog;

    void CreateWidgets();

    //SELECTION
    void ClickEvent(vtkObject* a_selectedObject, unsigned long notUseduLong, void* notUsedVoid);
    void SelectAll();
    void UnselectAll();
    void keyPressEvent(QKeyEvent * keyEvent);

    //PLACING WIDGETS
    void printColNumber(unsigned int colNumber);
    int getNumberOfColumns();
    int getNumberOfRows(unsigned int colNumber);
    void placeWidgetInArea(unsigned int colNumber);
    void resizeWidgetInArea();
    void resizeEvent(QResizeEvent* Qevent);

    // DRAG & DROP FILES
    void dragEnterEvent(QDragEnterEvent *Qevent);
    void dropEvent(QDropEvent* Qevent);

    // UPDATES & DISPLAY INFO
    void UpdateColorbar_QT();
    void UpdateArrowPosition();
    void displayInfo();
    void displayAttribute();


  protected slots:

    //QUIT
    void slotExit();
    void on_pushButton_displayTools_clicked();

    //FILE
    void openDirectory();
    void openFiles();
    void loadCSV();
    void slot_itemsSelected(QFileInfoList fileList);
    void deleteAll();
    void deleteSelection();

    //OPTIONS
    void showCameraConfigWindow();
    void showBackgroundConfigWindow();
    void loadColorMap();
    void saveColorMap();

    //DISPLAY INFO RANGE
    void on_tabWidget_currentChanged(int index);

    //DISPLAY
    void on_radioButton_DISPLAY_all_toggled();
    void on_radioButton_DISPLAY_square_toggled();
    void on_spinBox_DISPLAY_columns_valueChanged();

    //SYNCHRO
    void on_radioButton_SYNC_realtime_toggled();
    void on_radioButton_SYNC_delayed_toggled();
    void on_pushButton_SYNC_all_clicked();
    void on_comboBox_SYNC_position_currentIndexChanged();
    void on_pushButton_SYNC_unselect_clicked();

    //VIEW (Axis + Camera)
    void on_toolButton_VIEW_P_clicked();
    void on_toolButton_VIEW_A_clicked();
    void on_toolButton_VIEW_L_clicked();
    void on_toolButton_VIEW_R_clicked();
    void on_toolButton_VIEW_S_clicked();
    void on_toolButton_VIEW_I_clicked();
    void on_pushButton_VIEW_reset_clicked();

    //BACKGROUND CONFIG
    void slot_selectedColor_valueChanged(QColor color);
    void slot_unselectedColor_valueChanged(QColor color);
    void slot_textColor_valueChanged(QColor color);

    //CAMERA CONFIG
    void slot_position_x_valueChanged(double arg1);
    void slot_position_y_valueChanged(double arg1);
    void slot_position_z_valueChanged(double arg1);
    void slot_focalpoint_x_valueChanged(double arg1);
    void slot_focalpoint_y_valueChanged(double arg1);
    void slot_focalpoint_z_valueChanged(double arg1);
    void slot_viewup_vx_valueChanged(double arg1);
    void slot_viewup_vy_valueChanged(double arg1);
    void slot_viewup_vz_valueChanged(double arg1);
    void slot_scale_valueChanged(double arg1);
    void slot_newCameraConfig(cameraConfigStruct cam);


    //COLORMAP
    void on_comboBox_VISU_attribute_currentIndexChanged();
    void on_spinBox_VISU_min_valueChanged(double arg1);
    void on_pushButton_VISU_resetRange_clicked();
    void on_spinBox_VISU_max_valueChanged(double arg1);
    void on_spinBox_VISU_position_valueChanged(double arg1);
    void on_pushButton_VISU_delete_clicked();
    void on_pushButton_VISU_add_clicked();
    void on_pushButton_VISU_reset_clicked();
    //slots for gradView signals
    void slot_gradArrow_moved(qreal newPos);
    void slot_gradArrow_selected(qreal newPos);
    void slot_gradArrow_doubleClicked();
    void slot_no_gradArrow_selected();


    void UpdateCameraConfig();
  signals:
    void sig_updateCameraConfig(cameraConfigStruct cameraConfig);

private slots:
};

#endif

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
#ifndef SPV_EXTENSION
#include <vtkGL2PSExporter.h>
#endif

class ShapePopulationQT : public QMainWindow, public Ui::ShapePopulationQT, public ShapePopulationBase
{
    Q_OBJECT
    
public:
    
    ShapePopulationQT();
    ~ShapePopulationQT();
    
    void loadVTKFilesCLP(QFileInfoList a_fileList);
    void loadCSVFileCLP(QFileInfo file);
    void loadVTKDirCLP(QDir vtkDir);
    void loadColorMapCLP(std::string a_filePath);
    void loadCameraCLP(std::string a_filePath);
    
    
protected:
    
    bool m_toolsDisplayed;
    bool m_updateOnPositionChanged;
    bool m_updateOnAttributeChanged;
    bool m_linkCoordinate;
    unsigned int m_numberOfMeshes;
    QString m_lastDirectory;
    QString m_colormapDirectory;
    QString m_exportDirectory;
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
    int getNumberOfColumns();
    int getNumberOfRows(unsigned int colNumber);
    void placeWidgetInArea(unsigned int colNumber);
    void resizeWidgetInArea();
    void resizeEvent(QResizeEvent* Qevent);
    
    // DRAG & DROP FILES
    void dragEnterEvent(QDragEnterEvent *Qevent);
    void dropEvent(QDropEvent* Qevent);
    
    // UPDATES & DISPLAY INFO
    void updateColorbar_QT();
    void UpdateColorMapByDirection_QT();
    void UpdateColorMapByAbsoluteDirection_QT();
    void computeRangeDirection_QT();
    void computeNorm_QT();
    void updateAttribute_QT();
    void updateArrowPosition();
    void updateInfo_QT();
    
    
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
    void on_spinBox_DISPLAY_columns_editingFinished();
    void on_checkBox_displayColorbar_toggled(bool checked);
    void on_checkBox_displayAttribute_toggled(bool checked);
    void on_checkBox_displayMeshName_toggled(bool checked);
    
    //SYNCHRO
    void on_radioButton_SYNC_realtime_toggled();
    void on_radioButton_SYNC_delayed_toggled();
    void on_pushButton_SYNC_all_clicked();
    void on_pushButton_SYNC_unselect_clicked();
    
    //VIEW (Axis + Camera)
    void on_toolButton_VIEW_P_clicked();
    void on_toolButton_VIEW_A_clicked();
    void on_toolButton_VIEW_L_clicked();
    void on_toolButton_VIEW_R_clicked();
    void on_toolButton_VIEW_S_clicked();
    void on_toolButton_VIEW_I_clicked();
    void on_pushButton_VIEW_reset_clicked();
    void on_comboBox_alignment_currentIndexChanged();
    
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
    void on_spinBox_VISU_min_editingFinished();
    void on_pushButton_VISU_resetRange_clicked();
    void on_spinBox_VISU_max_editingFinished();
    
    void on_spinBox_VISU_min_AxisX_editingFinished();
    void on_spinBox_VISU_max_AxisX_editingFinished();
    void on_pushButton_VISU_resetRange_AxisX_clicked();

    void on_spinBox_VISU_min_AxisY_editingFinished();
    void on_spinBox_VISU_max_AxisY_editingFinished();
    void on_pushButton_VISU_resetRange_AxisY_clicked();

    void on_spinBox_VISU_min_AxisZ_editingFinished();
    void on_spinBox_VISU_max_AxisZ_editingFinished();
    void on_pushButton_VISU_resetRange_AxisZ_clicked();

    void on_pushButton_VISU_link_coordinate_clicked();
    
    void on_spinBox_VISU_position_valueChanged(double arg1);
    void on_pushButton_VISU_delete_clicked();
    void on_pushButton_VISU_add_clicked();
    void on_pushButton_VISU_reset_clicked();
    void on_radioButton_displayColorMapByMagnitude_toggled(bool checked);
    void on_radioButton_displayColorMapByDirection_toggled(bool checked);
    void on_checkBox_displayAbsoluteColorMapByDirection_toggled(bool checked);
    //slots for gradView signals
    void slot_gradArrow_moved(qreal newPos);
    void slot_gradArrow_selected(qreal newPos);
    void slot_gradArrow_doubleClicked();
    void slot_no_gradArrow_selected();
    
    //VECTORS
    void on_checkBox_displayVectors_toggled(bool checked);
    void on_radioButton_displayVectorsbyMagnitude_toggled(bool checked);
    void on_radioButton_displayVectorsbyDirection_toggled(bool checked);
    void on_radioButton_displayVectorsByAbsoluteDirection_toggled(bool checked);
    void on_slider_meshOpacity_valueChanged(int value);
    void on_slider_vectorScale_valueChanged(int value);
    void on_slider_arrowDens_valueChanged(int value);
    
    //EXPORT
#ifndef SPV_EXTENSION
    void exportToPDF();
    void exportToPS();
    void exportToEPS();
    void exportToTEX();
    void exportToSVG();
    int getExportDirectory();
    void exportTo(int fileFormat);
#endif
    void showNoExportWindow();
    
    void UpdateCameraConfig();
signals:
    void sig_updateCameraConfig(cameraConfigStruct cameraConfig);
    
};

#endif

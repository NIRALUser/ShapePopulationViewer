#ifndef SHAPEPOPULATIONQT_H
#define SHAPEPOPULATIONQT_H

// local
#include "ui_ShapePopulationQT.h"
#include "ShapePopulationBase.h"
#include "ShapePopulationViewerConfig.h"
#include "gradientWidgetQT.h"
#include "cameraDialogQT.h"
#include "backgroundDialogQT.h"
#include "CSVloaderQT.h"
#include "timeSeriesLoaderQT.h"
#include "customizeColorMapByDirectionDialogQT.h"
#include <iostream>
#include <vtkInteractorStyleTrackballCamera.h>

// MRML includes
class vtkMRMLModelNode;

// QT
#include <QWidget>
#  include <QVTKOpenGLNativeWidget.h>
class QActionGroup;
#include <QFileDialog>              //Open directory/files
#include <QFileInfo>                //Use Files
#include <QKeyEvent>                //KeyPressEvent
#include <QStandardItemModel>       //Data Array
#include <QColorDialog>             //ColorPicker
#include <vtkDelimitedTextReader.h> //CSVloader
#include <QUrl>                     //DropFiles
#include <QMimeData>

#include <vtkOrientationMarkerWidget.h>


#include <QDebug>
#ifdef ShapePopulationViewer_HAS_EXPORT_SUPPORT
#include <vtkGL2PSExporter.h>
#endif

class ShapePopulationQT : public QWidget, public Ui::ShapePopulationQT, public ShapePopulationBase
{
    Q_OBJECT
public:

    ShapePopulationQT(QWidget* parent=0);
    ~ShapePopulationQT();

    void loadVTKFilesCLP(QFileInfoList a_fileList);
    void loadSRepFilesCLP(QFileInfoList a_fileList);
    void loadFiducialFilesCLP(QFileInfoList a_fileList);
    void loadModel(vtkMRMLModelNode* modelNode);
    void loadModel(vtkPolyData* polyData, const QString& modelName);
    void loadCSVFileCLP(QFileInfo file);
    void loadTimeSeriesCLP(QFileInfo file);
    void loadVTKDirCLP(QDir vtkDir);
    void loadColorMapCLP(std::string a_filePath);
    void loadCameraCLP(std::string a_filePath);

    static QStringList parseCSVFile(const QString& csvFilePath);

protected:

    bool m_toolsDisplayed;
    bool m_updateOnPositionChanged;
    bool m_updateOnAttributeChanged;
    bool m_noChange;
    bool m_firstDisplayVector;
    unsigned int m_numberOfMeshes;
    QString m_lastDirectory;
    QString m_colormapDirectory;
    QString m_exportDirectory;
    QString m_pathSphere;
    typedef QVTKOpenGLNativeWidget VTKWidgetType;
    std::vector<VTKWidgetType *> m_widgetList;
    QList<QFileInfoList> m_timeSeries;
    cameraDialogQT * m_cameraDialog;
    backgroundDialogQT * m_backgroundDialog;
    CSVloaderQT * m_CSVloaderDialog;
    timeSeriesLoaderQT * m_timeSeriesLoaderDialog;
    customizeColorMapByDirectionDialogQT* m_customizeColorMapByDirectionDialog;

    void CreateWidgets(const QFileInfoList& files);
    void CreateWidgets(const QList<vtkRenderWindow*>& renderWindows, bool removeExistingWidgets=true);


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
    void updateAttribute_QT();
    void updateArrowPosition();
    void updateInfo_QT();

    protected slots:

    void on_pushButton_displayTools_clicked();

    //FILE
    void openDirectory();
    void openFiles();
    void openSRepFiles();
    void openFiducialFiles();
    void loadCSV();
    void loadTimeSeries();
    void slot_itemsSelected(QFileInfoList fileList);
    void slot_timeSeriesSelected(QList<QFileInfoList> fileList);
    void slot_timeIndicesChanged(double index);
    void deleteAll();
    void deleteSelection();

    //OPTIONS
    void showCameraConfigWindow();
    void showBackgroundConfigWindow();
    void loadColorMap();
    void saveColorMap();
    void showCustomizeColorMapByDirectionConfigWindow();

    //DISPLAY INFO RANGE
    void on_tabWidget_currentChanged(int index);

    //DISPLAY
    void on_radioButton_DISPLAY_all_toggled();
    void on_radioButton_DISPLAY_square_toggled();
    void on_spinBox_DISPLAY_columns_valueChanged(int value = -1);
    void on_checkBox_displayColorbar_toggled(bool checked);
    void on_checkBox_displayAttribute_toggled(bool checked);
    void on_checkBox_displayMeshName_toggled(bool checked);
    void on_checkBox_displaySphere_toggled(bool checked);


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
    void on_comboBox_alignment_currentIndexChanged(int value = -1);

    //BACKGROUND CONFIG
    void slot_selectedColor_valueChanged(QColor color);
    void slot_unselectedColor_valueChanged(QColor color);
    void slot_textColor_valueChanged(QColor color);

    // CUSTOMIZE COLOR MAP BY DIRECTION
    void slot_sameColor_valueChanged(bool checked);
    void slot_complementaryColor_valueChanged(bool checked);
    void slot_plusXAxis_valueChanged(QColor color);
    void slot_plusYAxis_valueChanged(QColor color);
    void slot_plusZAxis_valueChanged(QColor color);

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
    void on_comboBox_VISU_attribute_currentIndexChanged(int value = -1);
    // range
    void on_spinBox_VISU_min_valueChanged(double min);
    void on_spinBox_VISU_max_valueChanged(double max);
    void on_pushButton_VISU_resetRange_clicked();
    void on_spinBox_VISU_max_Dir_valueChanged(double newXmax);
    void on_pushButton_VISU_resetRange_Dir_clicked();
    // arrows of the gradient widget
    void on_spinBox_VISU_position_valueChanged(double arg1);
#ifndef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    void on_pushButton_VISU_delete_clicked();
    void on_pushButton_VISU_add_clicked();
#endif
    void on_pushButton_VISU_reset_clicked();
    // color map
    void on_radioButton_displayColorMapByMagnitude_toggled(bool checked);
    void on_radioButton_displayColorMapByDirection_toggled(bool checked);

    //slots for gradView signals
    void slot_gradArrow_moved(qreal newPos);
    void slot_gradArrow_selected(qreal newPos);
#ifndef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    void slot_gradArrow_doubleClicked();
#endif
    void slot_no_gradArrow_selected();

    //VECTORS
    void on_slider_meshOpacity_valueChanged(int value);
    void on_slider_vectorScale_valueChanged(int value);
    void on_slider_arrowDens_valueChanged(int value);
    void on_checkBox_displayVectors_toggled(bool checked);
    void on_radioButton_displayVectorsbyMagnitude_toggled(bool checked);
    void on_radioButton_displayVectorsbyDirection_toggled(bool checked);

    //EXPORT
#ifdef ShapePopulationViewer_HAS_EXPORT_SUPPORT
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
    void sig_axisColor_value(axisColorStruct* axisColor, bool dialogOpen);
    void sig_backgroundColor_valueChanged(double backgroundColor_red, double backgroundColor_green, double backgroundColor_blue, bool dialogOpen);
    void sig_resetColor();
    void sig_loadTimeSeries(bool slider_enabled);

private:
    QActionGroup* m_exportActions;
    QActionGroup* m_optionsActions;
};

#endif

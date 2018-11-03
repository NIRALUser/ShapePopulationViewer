#ifndef CUSTOMIZECOLORMAPBYDIRECTIONDIALOGQT_H
#define CUSTOMIZECOLORMAPBYDIRECTIONDIALOGQT_H

#include "ShapePopulationBase.h"
#include "axisColorStruct.h"

#include <QDialog>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointData.h>
#include <vtkMath.h>
#include <vtkArrowSource.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>
#include <iostream>
#ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLWIDGET
# ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLNATIVEWIDGET
#  include <QVTKOpenGLNativeWidget.h>
# else
#  include <QVTKOpenGLWidget.h>
# endif
#else
# include <QVTKWidget.h>
#endif
#include <QColorDialog>
#include <QFrame>

namespace Ui {
class customizeColorMapByDirectionDialogQT;
}

class customizeColorMapByDirectionDialogQT : public QDialog, public ShapePopulationBase
{
    Q_OBJECT

public:
    explicit customizeColorMapByDirectionDialogQT(QWidget *parent = 0);
    ~customizeColorMapByDirectionDialogQT();
    void AxisColor();

protected:
    std::vector< vtkSmartPointer<vtkActor> > m_arrowActors;
    std::vector< vtkSmartPointer<vtkTextActor> > m_labelActors;
#ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLWIDGET
# ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLNATIVEWIDGET
    typedef QVTKOpenGLNativeWidget VTKWidgetType;
# else
    typedef QVTKOpenGLWidget VTKWidgetType;
# endif
#else
    typedef QVTKWidget VTKWidgetType;
#endif
    VTKWidgetType * widget;
    bool m_sameColor;
    bool m_complementaryColor;
    std::vector< QFrame* > m_frameAxis;
    std::vector< QColor > m_axisColor;
    double m_backgroundColor[3];

protected slots:
    void on_radioButton_sameColor_toggled(bool checked);
    void on_radioButton_complementaryColor_toggled(bool checked);
    void on_pushButton_pick_plusXAxis_clicked();
    void on_pushButton_pick_plusYAxis_clicked();
    void on_pushButton_pick_plusZAxis_clicked();
    void on_pushButton_default_clicked();
    void updateAxisColor_value(axisColorStruct* axisColor, bool dialogOpen);
    void updateBackgroundColor_valueChanged(double backgroundColor_red, double backgroundColor_green, double backgroundColor_blue, bool dialogOpen);
    void resetColor();


signals:
    void sig_sameColor_valueChanged(bool checked);
    void sig_complementaryColor_valueChanged(bool checked);
    void sig_plusXAxis_valueChanged(QColor color);
    void sig_plusYAxis_valueChanged(QColor color);
    void sig_plusZAxis_valueChanged(QColor color);

private:
    Ui::customizeColorMapByDirectionDialogQT *ui;
};

#endif // CUSTOMIZECOLORMAPBYDIRECTIONDIALOGQT_H


// ShapePopulationViewer includes
#include "ShapePopulationColorMapIO.h"
#include "ShapePopulationGradientWidgetQT.h"

// CTK includes
#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
#include <ctkTransferFunction.h>
#include <ctkTransferFunctionGradientItem.h>
#include <ctkTransferFunctionControlPointsItem.h>
#include <ctkVTKColorTransferFunction.h>
#include <ctkVTKScalarsToColorsView.h>

// VTK includes
#include <vtkAxis.h>
#include <vtkColorTransferFunction.h>
#include <vtkContextScene.h>
#include <vtkControlPointsItem.h>
#include <vtkPlot.h>
#include <vtkSmartPointer.h>

// Qt includes
#include <QDebug>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#endif

class ShapePopulationGradientWidgetQTPrivate
{
public:
    ShapePopulationGradientWidgetQTPrivate();

#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    vtkSmartPointer<vtkColorTransferFunction> CTF;
    QDoubleSpinBox* XSpinBox;
#endif
};

ShapePopulationGradientWidgetQTPrivate::ShapePopulationGradientWidgetQTPrivate()
{
}

ShapePopulationGradientWidgetQT::ShapePopulationGradientWidgetQT(QWidget* parent)
    : Superclass(parent), d_ptr( new ShapePopulationGradientWidgetQTPrivate )
{
#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    Q_D(ShapePopulationGradientWidgetQT);
    this->setHorizontalSliderVisible(false);
    this->setVerticalSliderVisible(false);
    this->setTopWidgetsVisible(true);

    d->CTF = vtkSmartPointer<vtkColorTransferFunction>::New();
    this->view()->addColorTransferFunction(d->CTF);

    this->qvtkConnect(this->currentControlPointsItem(), vtkControlPointsItem::CurrentPointChangedEvent,
                      this, SLOT(onCurrentPointChangedEvent(vtkObject*)));
    this->qvtkConnect(this->currentControlPointsItem(), vtkCommand::ModifiedEvent,
                      this, SLOT(onCurrentPointModifiedEvent(vtkObject*)));

    this->reset();
#endif
}

ShapePopulationGradientWidgetQT::~ShapePopulationGradientWidgetQT()
{
}

#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION

void ShapePopulationGradientWidgetQT::loadColorPointList(QString a_filePath, std::vector<colorPointStruct> * a_colorPointList)
{
    ShapePopulationColorMapIO::loadColorPointList(a_filePath, a_colorPointList, this);
    this->setAllColors(a_colorPointList);
}

void ShapePopulationGradientWidgetQT::saveColorPointList(QString a_filePath)
{
    //Get ColorPointList
    std::vector<colorPointStruct> colorPointList;
    getAllColors(&colorPointList);
    ShapePopulationColorMapIO::saveColorPointList(a_filePath, colorPointList, this);
}

bool ShapePopulationGradientWidgetQT::getAllColors(std::vector<colorPointStruct> * colorPointList)
{
    Q_D(ShapePopulationGradientWidgetQT);
    colorPointList->clear();

    for(int idx = 0; idx < d->CTF->GetSize(); ++idx)
    {
        double nodeValue[6];
        d->CTF->GetNodeValue(idx, nodeValue);
        colorPointStruct colorPoint(nodeValue[0], nodeValue[1], nodeValue[2], nodeValue[3]);
        colorPointList->push_back(colorPoint);
    }
    return d->CTF->GetSize() > 0;
}

void ShapePopulationGradientWidgetQT::setAllColors(std::vector<colorPointStruct> * colorPointList)
{
    Q_D(ShapePopulationGradientWidgetQT);
    d->CTF->RemoveAllPoints();
    for(unsigned int idx = 0 ; idx < colorPointList->size() ; idx++)
    {
        colorPointStruct colorPoint = colorPointList->at(idx);
        d->CTF->AddRGBPoint(colorPoint.pos, colorPoint.r, colorPoint.g, colorPoint.b);
    }
}

void ShapePopulationGradientWidgetQT::setFocusPosition(const qreal newPos)
{
    vtkIdType currentPointId = this->currentControlPointsItem()->GetCurrentPoint();
    double point[4] = {0.0};
    this->currentControlPointsItem()->GetControlPoint(currentPointId, point);
    point[0] = newPos;
    this->currentControlPointsItem()->SetControlPoint(currentPointId, point);
}

qreal ShapePopulationGradientWidgetQT::getFocusPosition()
{
    vtkIdType currentPointId = this->currentControlPointsItem()->GetCurrentPoint();
    if (currentPointId == -1)
    {
        return 0.0;
    }
    double point[4] = {0.0};
    this->currentControlPointsItem()->GetControlPoint(currentPointId, point);
    return point[0];
}

void ShapePopulationGradientWidgetQT::setFocusIndex(int index)
{
    if (index < -1 || index >= this->currentControlPointsItem()->GetNumberOfPoints())
    {
        return;
    }
    this->currentControlPointsItem()->SetCurrentPoint(index);
}

int ShapePopulationGradientWidgetQT::getFocusIndex()
{
    return static_cast<int>(this->currentControlPointsItem()->GetCurrentPoint());
}

void ShapePopulationGradientWidgetQT::enable(std::vector<colorPointStruct> *a_colorPointList)
{
    this->setEnabled(true);
    this->setAllColors(a_colorPointList);
}

void ShapePopulationGradientWidgetQT::disable()
{
    this->setEnabled(false);
}

int ShapePopulationGradientWidgetQT::getNumberOfArrows()
{
    Q_D(ShapePopulationGradientWidgetQT);
    return d->CTF->GetSize();
}

void ShapePopulationGradientWidgetQT::reset()
{
    Q_D(ShapePopulationGradientWidgetQT);
    d->CTF->RemoveAllPoints();

    double range_min = 0.0;
    double range_max = 1.0;
    double center = (range_max - range_min) / 2.0;

    // Elements: {leftMin, leftMax, bottomMin, bottomMax, rightMin, rightMax, topMin, topMax}
    // (probably according to vtkAxis::Location)
    double chartBounds[8] = {0.0};
    this->view()->chartBounds(chartBounds);
    chartBounds[2] = range_min;
    chartBounds[3] = range_max;
    this->view()->setChartUserBounds(chartBounds);
    this->view()->update();
    this->view()->setAxesToChartBounds();

    {
        QColor color = Qt::green;
        d->CTF->AddRGBPoint(range_min, color.redF(), color.greenF(), color.blueF());
    }
    {
        QColor color = Qt::yellow;
        d->CTF->AddRGBPoint(center, color.redF(), color.greenF(), color.blueF());
    }
    {
        QColor color = Qt::red;
        d->CTF->AddRGBPoint(range_max, color.redF(), color.greenF(), color.blueF());
    }
}

void ShapePopulationGradientWidgetQT::onCurrentPointChangedEvent(vtkObject* caller)
{
    vtkControlPointsItem* controlPoints = vtkControlPointsItem::SafeDownCast(caller);
    long newPoint = this->getFocusIndex();
    if (!controlPoints || newPoint <= -1)
        {
        emit noSelectionSignal();
        return;
        }
    emit arrowSelectedSignal(this->getFocusPosition());
}

void ShapePopulationGradientWidgetQT::onCurrentPointModifiedEvent(vtkObject* caller)
{
    vtkControlPointsItem* controlPoints = vtkControlPointsItem::SafeDownCast(caller);
    long newPoint = this->getFocusIndex();
    if (!controlPoints || newPoint <= -1)
      {
      return;
      }
    emit arrowMovedSignal(this->getFocusPosition());
}
#endif

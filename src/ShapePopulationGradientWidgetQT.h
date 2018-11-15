#ifndef ShapePopulationGradientWidgetQT_h
#define ShapePopulationGradientWidgetQT_h

#include "ShapePopulationStruct.h"
#include "ShapePopulationViewerConfig.h"

#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
# include <ctkVTKScalarsToColorsWidget.h>
#else
# include "gradientWidgetQT.h"
#endif

// Qt includes
#include <QColor>

// STD includes
#include <vector>

class ShapePopulationGradientWidgetQTPrivate;

class ShapePopulationGradientWidgetQT :
#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    public ctkVTKScalarsToColorsWidget
#else
    public gradientWidgetQT
#endif
{
    Q_OBJECT
public:
#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    typedef ctkVTKScalarsToColorsWidget Superclass;
#else
    typedef gradientWidgetQT Superclass;
#endif
    ShapePopulationGradientWidgetQT(QWidget* parent=0);
    virtual ~ShapePopulationGradientWidgetQT();

#ifdef ShapePopulationViewer_BUILD_SLICER_EXTENSION
    void loadColorPointList(QString a_filePath, std::vector<colorPointStruct> * a_colorPointList);
    void saveColorPointList(QString a_filePath);

    bool getAllColors(std::vector<colorPointStruct> * colorPointList);
    void setAllColors(std::vector<colorPointStruct> * colorPointList);

    void setFocusPosition(const qreal newPos);
    qreal getFocusPosition();

    void setFocusIndex(int index);
    int getFocusIndex();

    void enable(std::vector<colorPointStruct> *a_colorPointList);
    void disable();

    int getNumberOfArrows();

public slots:
    void reset();
    void onCurrentPointChangedEvent(vtkObject* controlPointsItem);
    void onCurrentPointModifiedEvent(vtkObject* controlPointsItem);

signals:
    void arrowMovedSignal(const qreal pos);
    void arrowSelectedSignal(const qreal pos);
    void noSelectionSignal();

#endif

protected:
    QScopedPointer<ShapePopulationGradientWidgetQTPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(ShapePopulationGradientWidgetQT)
    Q_DISABLE_COPY(ShapePopulationGradientWidgetQT);
};


#endif

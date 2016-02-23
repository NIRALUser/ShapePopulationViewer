#include "testLabelColor.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testLabelColor(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename);

    shapePopulationBase->m_displayColorMapByDirection.push_back(false);
    shapePopulationBase->m_displayVectorsByDirection.push_back(false);

    std::vector < double* > labelColor;

    // Test1. Color of title in orange
    double color[3] = {1,0.5,0};
    labelColor.push_back(color);

    // Test2. Color of title in blue
    color[0] = 0;
    color[1] = 0;
    color[2] = 1;
    labelColor.push_back(color);

    for(int i = 0; i < 2; i++)
    {
        // Call of the function that must be test
        shapePopulationBase->setLabelColor(labelColor[i]);

        // Test if the result obtained is correct:
        vtkSmartPointer<vtkPropCollection> propCollection =  shapePopulationBase->m_windowsList[0]->GetRenderers()->GetFirstRenderer()->GetViewProps();

        double meshNameColor[3];
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        vtkSmartPointer<vtkTextProperty> cornerProperty = cornerAnnotation->GetTextProperty();
        cornerProperty->GetColor(meshNameColor);
        if(meshNameColor[0] != labelColor[i][0] || meshNameColor[1] != labelColor[i][1] || meshNameColor[2] != labelColor[i][2])
        {
            return 1;
        }

        double attributeNameColor[3];
        viewPropObject = propCollection->GetItemAsObject(3);
        cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        cornerProperty = cornerAnnotation->GetTextProperty();
        cornerProperty->GetColor(attributeNameColor);
        if(attributeNameColor[0] != labelColor[i][0] || attributeNameColor[1] != labelColor[i][1] || attributeNameColor[2] != labelColor[i][2])
        {
            return 1;
        }

        double scalarColor[3];
        viewPropObject = propCollection->GetItemAsObject(4);
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)viewPropObject;
        vtkSmartPointer<vtkTextProperty> labelProperty = scalarBar->GetLabelTextProperty();
        labelProperty->GetColor(scalarColor);
        if(scalarColor[0] != labelColor[i][0] || scalarColor[1] != labelColor[i][1] || scalarColor[2] != labelColor[i][2])
        {
            return 1;
        }
    }
    return 0;
}





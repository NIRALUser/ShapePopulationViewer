
#include "testDisplayAttribute.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testDisplayAttribute(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    int nbMesh = 2;
    shapePopulationBase->m_windowsList.clear();
    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->CreateNewWindow(filename);
    }

    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        if(!cornerAnnotation->GetVisibility()) return 1;
    }

    /// TEST 1.
    /// Call the fonction to set the attribute name not visible
    /// Wanted result: attribute name is not visible

    // Call of the function that must be test
    shapePopulationBase->displayAttribute(false);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        if(cornerAnnotation->GetVisibility()) return 1;
    }

    /// TEST 2.
    /// Call the fonction to set the attribute name not visible
    /// Wanted result: attribute name is not visible

    // Call of the function that must be test
    shapePopulationBase->displayAttribute(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        if(!cornerAnnotation->GetVisibility()) return 1;
    }

    return 0;
}





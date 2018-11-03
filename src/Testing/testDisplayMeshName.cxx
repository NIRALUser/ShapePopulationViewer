#include "testDisplayMeshName.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testDisplayMeshName(std::string filename)
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
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);
        if(!cornerAnnotation->GetVisibility()) return 1;
    }

    /// TEST 1.
    /// Call the fonction to set the mesh name not visible
    /// Wanted result: mesh name is not visible

    // Call of the function that must be test
    shapePopulationBase->displayMeshName(false);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);
        if(cornerAnnotation->GetVisibility()) return 1;
    }

    /// TEST 2.
    /// Call the fonction to set the mesh name not visible
    /// Wanted result: mesh name is not visible

    // Call of the function that must be test
    shapePopulationBase->displayMeshName(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);
        if(!cornerAnnotation->GetVisibility()) return 1;
    }

    return 0;
}





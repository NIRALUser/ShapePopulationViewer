#include "testDisplayColorbar.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testDisplayColorbar(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    int nbMesh = 2;
    shapePopulationBase->m_windowsList.clear();
    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->CreateNewWindow(filename);
        shapePopulationBase->m_displayColorMapByMagnitude.push_back(true);
        shapePopulationBase->m_displayVectorsByMagnitude.push_back(true);
    }

    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(!scalarBar->GetVisibility()) return 1;
    }

    /// TEST 1.
    /// The color map by magnitude is displayed
    /// The color of vectors is the color by magnitude
    /// Call the fonction to set the scalar bar not visible
    /// Wanted result: scalar bar is not visible

    // Call of the function that must be test
    shapePopulationBase->displayColorbar(false);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(scalarBar->GetVisibility()) return 1;
    }

    /// TEST 2.
    /// The color map by magnitude is not displayed
    /// The color of vectors is not the color by magnitude
    /// Call the fonction to set the scalar bar visible
    /// Wanted result: scalar bar is not visible

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByMagnitude[i] = false;
        shapePopulationBase->m_displayVectorsByMagnitude[i] = false;
    }

    // Call of the function that must be test
    shapePopulationBase->displayColorbar(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(scalarBar->GetVisibility()) return 1;
    }

    /// TEST 3.
    /// The color map by magnitude is displayed
    /// The color of vectors is not the color by magnitude
    /// Call the fonction to set the scalar bar visible
    /// Wanted result: scalar bar is visible

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByMagnitude[i] = true;
    }

    // Call of the function that must be test
    shapePopulationBase->displayColorbar(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(!scalarBar->GetVisibility()) return 1;
    }
    /// TEST 4.
    /// The color map by magnitude is displayed
    /// The color of vectors is not the color by magnitude
    /// Call the fonction to set the scalar bar not visible
    /// Wanted result: scalar bar is visible

    // Call of the function that must be test
    shapePopulationBase->displayColorbar(false);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(scalarBar->GetVisibility()) return 1;
    }

    /// TEST 5.
    /// The color map by magnitude is not displayed
    /// The color of vectors is the color by magnitude
    /// Call the fonction to set the scalar bar visible
    /// Wanted result: scalar bar is visible

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByMagnitude[i] = true;
    }

    // Call of the function that must be test
    shapePopulationBase->displayColorbar(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkPropCollection* propCollection =  shapePopulationBase->m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(!scalarBar->GetVisibility()) return 1;
    }

    return 0;
}







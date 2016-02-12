#include "testDisplayVectors.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testDisplayVectors(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );
    std::ostringstream strs_mag;
    strs_mag.str("");
    strs_mag.clear();
    strs_mag << "PointToPointVector_mag" << std::endl;

    int nbMesh = 4;
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->m_selectedIndex.clear();
    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayVectors.push_back(false);
        shapePopulationBase->m_displayColorMapByMagnitude.push_back(true);
        shapePopulationBase->m_displayVectorsByMagnitude.push_back(false);
        shapePopulationBase->m_displayColorMapByDirection.push_back(false);
        shapePopulationBase->m_displayVectorsByDirection.push_back(false);
        shapePopulationBase->m_selectedIndex.push_back(i);
        shapePopulationBase->CreateNewWindow(filename);
        shapePopulationBase->m_meshList[i]->GetPolyData()->GetPointData()->SetActiveScalars(strs_mag.str().c_str());
    }

    shapePopulationBase->computeCommonAttributes();

    /// Initial state : the vectors are displayed
    for(int i = 0; i < nbMesh; i++)
    {
        vtkRenderWindow * window = shapePopulationBase->m_windowsList[shapePopulationBase->m_selectedIndex[i]];
        vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        if(!glyphActor->GetVisibility()) return 1;
    }

    /// TEST 1.
    /// Call the fonction to not display the vectors
    /// Wanted result: vectors not visible

    // Call of the function that must be test
    shapePopulationBase->displayVectors(false);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkRenderWindow * window = shapePopulationBase->m_windowsList[shapePopulationBase->m_selectedIndex[i]];
        vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        if(glyphActor->GetVisibility()) return 1;
    }

    /// TEST 2.
    /// Call the fonction to display the vectors on the selected meshes
    /// Wanted result: vectors visible for the selected meshes
    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
    }

    // Call of the function that must be test
    shapePopulationBase->displayVectors(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkRenderWindow * window = shapePopulationBase->m_windowsList[i];
        vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        if( (std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)) != (shapePopulationBase->m_selectedIndex.end()) )
        {
            if(!glyphActor->GetVisibility()) return 1;
        }
        else
        {
            if(glyphActor->GetVisibility()) return 1;
        }
    }

    /// TEST 3.
    /// Call the fonction to display the vectors on the selected meshes
    /// Wanted result: vectors visible for the selected meshes
    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.push_back(i);
    }

    // Call of the function that must be test
    shapePopulationBase->displayVectors(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkRenderWindow * window = shapePopulationBase->m_windowsList[i];
        vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        if(!glyphActor->GetVisibility()) return 1;
    }

    /// TEST 4.
    /// Call the fonction to not display the vectors on the selected meshes
    /// Wanted result: vectors not visible for the selected meshes
    for(int i = 1; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
    }

    // Call of the function that must be test
    shapePopulationBase->displayVectors(false);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkRenderWindow * window = shapePopulationBase->m_windowsList[i];
        vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        if( (std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)) != (shapePopulationBase->m_selectedIndex.end()) )
        {
            if(glyphActor->GetVisibility()) return 1;
        }
        else
        {
            if(!glyphActor->GetVisibility()) return 1;
        }
    }

    return 0;

}


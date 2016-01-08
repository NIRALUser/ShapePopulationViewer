#include "testMeshOpacity.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testMeshOpacity(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename);


    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_meshOpacity.push_back(0);

    // Call of the function that must be test
    shapePopulationBase->setMeshOpacity(0.5);

    // Test if the result obtained is correct
    vtkActorCollection * actors = shapePopulationBase->m_windowsList[0]->GetRenderers()->GetFirstRenderer()->GetActors();
    actors->InitTraversal();
    double meshOpacity = actors->GetNextActor()->GetProperty()->GetOpacity();

    if(meshOpacity == 0.5)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

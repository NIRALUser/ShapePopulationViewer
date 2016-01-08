#include "testVectorScale.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testVectorScale(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_vectorScale.push_back(0);
    shapePopulationBase->m_displayVectorsByDirection.push_back(false);
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename);

    // Call of the function that must be test
    shapePopulationBase->setVectorScale(50);


    // Test if the result obtained is correct
    int vectorScale = shapePopulationBase->m_glyphList[0]->GetScaleFactor();

    if(vectorScale == 50)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}






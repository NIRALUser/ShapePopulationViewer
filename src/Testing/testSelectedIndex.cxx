#include "testSelectedIndex.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testSelectedIndex(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_windowsList.clear();

    int nbMesh = 4;
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->m_selectedIndex.clear();
    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_selectedIndex.push_back(i);
        shapePopulationBase->CreateNewWindow(filename);
    }

    vtkSmartPointer<vtkRenderWindow> selectedWindow = vtkSmartPointer<vtkRenderWindow>::New();
    for(int i = 0; i < nbMesh; i++)
    {
        selectedWindow = shapePopulationBase->m_windowsList[shapePopulationBase->m_selectedIndex[i]];

        // Call of the function that must be test
        unsigned int index = shapePopulationBase->getSelectedIndex(selectedWindow);

        // Test if the result obtained is correct:
        if( index != shapePopulationBase->m_selectedIndex[i] ) return 1;
    }

    return 0;
}





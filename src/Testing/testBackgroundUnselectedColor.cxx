#include "testBackgroundUnselectedColor.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testBackgroundUnselectedColor(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename, /* testing = */ true);
    shapePopulationBase->CreateNewWindow(filename, /* testing = */ true);

    std::vector< double* > unselectedColor;
    /// Test1.
    /// Color of unselected window in orange
    /// Color of selected window in blue
    double color[3] = {1,0.5,0};
    shapePopulationBase->m_selectedColor[0] = 0;
    shapePopulationBase->m_selectedColor[1] = 0;
    shapePopulationBase->m_selectedColor[2] = 1;
    unselectedColor.push_back(color);

    /// Test2.
    /// Color of unselected window in blue
    /// Color of selected window in orange
    color[0] = 0;
    color[1] = 0;
    color[2] = 1;
    shapePopulationBase->m_selectedColor[0] = 1;
    shapePopulationBase->m_selectedColor[1] = 0.5;
    shapePopulationBase->m_selectedColor[2] = 0;
    unselectedColor.push_back(color);

    for(int i = 0; i < 2; i ++)
    {
        // Call of the function that must be test
        shapePopulationBase->setBackgroundUnselectedColor(unselectedColor[i]);

        // Test if the result obtained is correct:
        double colorSelectedBackground[3];
        shapePopulationBase->m_windowsList[0]->GetRenderers()->GetFirstRenderer()->GetBackground(colorSelectedBackground);
        if(colorSelectedBackground[0] != shapePopulationBase->m_selectedColor[0] || colorSelectedBackground[1] != shapePopulationBase->m_selectedColor[1] || colorSelectedBackground[2] != shapePopulationBase->m_selectedColor[2])
        {
            return 1;
        }

        double colorUnselectedBackground[3];
        shapePopulationBase->m_windowsList[1]->GetRenderers()->GetFirstRenderer()->GetBackground(colorUnselectedBackground);
        if(colorUnselectedBackground[0] != unselectedColor[i][0] || colorUnselectedBackground[1] != unselectedColor[i][1] || colorUnselectedBackground[2] != unselectedColor[i][2])
        {
            return 1;
        }
    }

    return 0;
}






#include "testBackgroundSelectedColor.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testBackgroundSelectedColor(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename);

    std::vector<double*> selectedColor;

    // Test1. Background cyan
    double color[3] = {0,1,1};
    selectedColor.push_back(color);

    // Test2. Background orange
    color[0] = 1;
    color[1] = 0.5;
    color[2] = 0;
    selectedColor.push_back(color);

    for(int i = 0; i < 2; i++)
    {
        // Call of the function that must be test
        shapePopulationBase->setBackgroundSelectedColor(selectedColor[i]);

        // Test if the result obtained is correct:
        double colorBackground[3];
        shapePopulationBase->m_windowsList[0]->GetRenderers()->GetFirstRenderer()->GetBackground(colorBackground);
        if(colorBackground[0] != selectedColor[i][0] || colorBackground[1] != selectedColor[i][1] || colorBackground[2] != selectedColor[i][2])
        {
            return 1;
        }
    }
    return 0;
}




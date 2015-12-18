//*************************************************************************************************//
// Test displayColorMapByMagnitude and displayColorMapByDirection in the class ShapePopulationBase //
//*************************************************************************************************//

#include <iostream>
#include <string>
#include <QApplication>
#include <QFileInfo>

#include "testDisplayColorMap.h"

int main(int argc, char *argv[])
{
    TestShapePopulationBase testShapePopulationBase;
    bool test = testShapePopulationBase.testDisplayColorMap( (std::string)argv[1], (std::string)argv[2] );

    if(!test) return 0;
    else return -1;
}

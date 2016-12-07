//***************************************************************************//
//      Test computeCommonAttributes in the class ShapePopulationBase        //
//***************************************************************************//

#include <iostream>
#include <string>
#include <QApplication>
#include <QFileInfo>

#include "testComputeCommonAttributes.h"

int main(int, char *argv[])
{
    TestShapePopulationBase testShapePopulationBase;

    bool test = testShapePopulationBase.testComputeCommonAttributes((std::string)argv[1], (std::string)argv[2]);

    if(!test) return 0;
    else return -1;
}

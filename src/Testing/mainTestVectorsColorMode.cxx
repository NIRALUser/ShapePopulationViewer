//*************************************************************************************************//
//      Test the color mode of vectors by using the two functions displayVectorsByMagnitude,       //
//    displayVectorsByDirection and UpdateVectorsByDirection in the class ShapePopulationBase      //
//*************************************************************************************************//

#include <iostream>
#include <string>
#include <QApplication>
#include <QFileInfo>

#include "testVectorsColorMode.h"

int main(int argc, char *argv[])
{
    TestShapePopulationBase testShapePopulationBase;
    bool test = testShapePopulationBase.testVectorsColorMode( (std::string)argv[1]);

    if(!test) return 0;
    else return -1;
}

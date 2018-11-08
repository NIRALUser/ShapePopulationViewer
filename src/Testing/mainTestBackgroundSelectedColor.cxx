//***************************************************************************//
//     Test setBackgroundSelectedColor in the class ShapePopulationBase      //
//***************************************************************************//

#include <iostream>
#include <string>
#include <QApplication>
#include <QFileInfo>

#include "testBackgroundSelectedColor.h"

int main(int /*argc*/, char *argv[])
{
    TestShapePopulationBase testShapePopulationBase;

    bool test = testShapePopulationBase.testBackgroundSelectedColor( (std::string)argv[1] );

    if(!test) return 0;
    else return -1;
}

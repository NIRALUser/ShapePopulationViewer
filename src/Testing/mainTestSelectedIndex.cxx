//***************************************************************************//
//          Test getSelectedIndex in the class ShapePopulationBase           //
//***************************************************************************//

#include <iostream>
#include <string>
#include <QApplication>
#include <QFileInfo>

#include "testSelectedIndex.h"

int main(int argc, char *argv[])
{
    TestShapePopulationBase testShapePopulationBase;

    bool test = testShapePopulationBase.testSelectedIndex( (std::string)argv[1] );

    if(!test) return 0;
    else return -1;
}

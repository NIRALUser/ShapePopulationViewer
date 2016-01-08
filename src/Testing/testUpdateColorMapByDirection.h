#ifndef TESTUPDATECOLORMAPBYDIRECTION_H
#define TESTUPDATECOLORMAPBYDIRECTION_H


#include "../src/ShapePopulationBase.h"
#include <math.h>

class TestShapePopulationBase
{
public:
    TestShapePopulationBase();

    bool testUpdateColorMapByDirection(std::string filename, std::string filenameExpectedResult);
};

#endif // TESTUPDATECOLORMAPBYDIRECTION_H

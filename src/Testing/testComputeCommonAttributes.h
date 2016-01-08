#ifndef TESTCOMPUTECOMMONATTRIBUTES_H
#define TESTCOMPUTECOMMONATTRIBUTES_H


#include "../src/ShapePopulationBase.h"
#include <math.h>

class TestShapePopulationBase
{
public:
    TestShapePopulationBase();

    bool testComputeCommonAttributes(std::string filename, std::string filenameExpectedResult);

};

#endif // TESTCOMPUTECOMMONATTRIBUTES_H

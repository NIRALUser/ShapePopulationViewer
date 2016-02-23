#include "testComputeCommonRange.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testComputeCommonRange(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_selectedIndex.push_back(0);
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->CreateNewWindow(filename);

    std::ostringstream strs;
    strs.str("");
    strs.clear();
    strs << "PointToPointVector_mag" << std::endl;

    // Call of the function that must be test
    double *commonRange = shapePopulationBase->computeCommonRange(strs.str().c_str(),shapePopulationBase->m_selectedIndex);

    // Test if the result obtained is correct
    if(fabs(commonRange[1] - 5.69522) > 0.00001 ) return 1;
    if(fabs(commonRange[0] - 0.25032) > 0.00001 ) return 1;
    return 0;
}





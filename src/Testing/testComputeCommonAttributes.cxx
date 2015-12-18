
#include "testComputeCommonAttributes.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testComputeCommonAttributes(std::string filename1, std::string filename2)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_windowsList.clear();

    // Data 1.
    shapePopulationBase->CreateNewWindow(filename1);
    std::vector< const char* > attributes1;
    attributes1.push_back("AbsoluteMagNormDirDistance");
    attributes1.push_back("AbsolutePointToPointDistance");
    attributes1.push_back("Color_Map_Phi");
    attributes1.push_back("Color_Map_Theta");
    attributes1.push_back("MagNormVector");
    attributes1.push_back("Normals");
    attributes1.push_back("PointToPointAlongX");
    attributes1.push_back("PointToPointAlongY");
    attributes1.push_back("PointToPointAlongZ");
    attributes1.push_back("PointToPointVector");
    attributes1.push_back("SignedMagNormDirDistance");
    attributes1.push_back("SignedPointToPointDistance");
    int numAttributes = shapePopulationBase->m_meshList.at(0)->GetAttributeList().size();

    if(numAttributes != (int)attributes1.size()) return 1;

    for(int i = 0; i < numAttributes; i++)
    {
        if(attributes1[i] != shapePopulationBase->m_meshList.at(0)->GetAttributeList().at(i)) return 1;
    }

    // Data 2.
    shapePopulationBase->CreateNewWindow(filename2);
    std::vector< const char* > attributes2;
    attributes2.push_back("Color_Map_Phi");
    attributes2.push_back("Color_Map_Theta");
    attributes2.push_back("Normals");
    attributes2.push_back("Original");
    attributes2.push_back("PointToPointAlongX");
    attributes2.push_back("PointToPointAlongY");
    attributes2.push_back("PointToPointAlongZ");
    attributes2.push_back("PointToPointVector");
    attributes2.push_back("Signed");

    numAttributes = shapePopulationBase->m_meshList.at(1)->GetAttributeList().size();

    if(numAttributes != (int)attributes2.size()) return 1;

    for(int i = 0; i < numAttributes; i++)
    {
        if(attributes2[i] != shapePopulationBase->m_meshList.at(1)->GetAttributeList().at(i)) return 1;
    }

    // Call of the function that must be test
    shapePopulationBase->computeCommonAttributes();

    // Test if the result obtained is correct:
    std::vector< const char* > commonAttributes;
    commonAttributes.push_back("Color_Map_Phi");
    commonAttributes.push_back("Color_Map_Theta");
    commonAttributes.push_back("Normals");
    commonAttributes.push_back("PointToPointAlongX");
    commonAttributes.push_back("PointToPointAlongY");
    commonAttributes.push_back("PointToPointAlongZ");
    commonAttributes.push_back("PointToPointVector");

    if(shapePopulationBase->m_commonAttributes.size() != commonAttributes.size()) return 1;

    for(unsigned int i = 0; i < shapePopulationBase->m_commonAttributes.size(); i++)
    {
        if(shapePopulationBase->m_commonAttributes[i] != commonAttributes[i]) return 1;
    }

    return 0;
}


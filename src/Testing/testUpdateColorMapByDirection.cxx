#include "testUpdateColorMapByDirection.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testUpdateColorMapByDirection(std::string filename, std::string filenameExpectedResult)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );
    std::ostringstream strs_mag;
    strs_mag.str("");
    strs_mag.clear();
    strs_mag << "PointToPointVector_mag" << std::endl;

    std::ostringstream strs_dir;
    strs_dir.str("");
    strs_dir.clear();
    strs_dir << "PointToPointVector_ColorByDirection" << std::endl;

    int nbTest = 4;
    shapePopulationBase->m_selectedIndex.clear();
    shapePopulationBase->m_windowsList.clear();
    for(int i = 0; i < nbTest; i++)
    {
        shapePopulationBase->m_selectedIndex.push_back(i);
        shapePopulationBase->CreateNewWindow(filename, /* testing = */ true);
    }

    /// TEST BY CHANGING THE COLOR OF THE MAIN AXIS ///

    // MAGNITUDE
    double *commonRange = shapePopulationBase->computeCommonRange(strs_mag.str().c_str(),shapePopulationBase->m_selectedIndex);
    magnitudStruct* magnitude = new magnitudStruct;
    magnitude->min = commonRange[0];
    magnitude->max = commonRange[1];
    shapePopulationBase->m_magnitude.push_back(magnitude);

    // COLOR OF AXIS
    /// Test 1.
    /// - complementary color for positive and negative axis
    /// - X: Red Y: Green Z: Blue
    axisColorStruct* axisColor = new axisColorStruct;
    axisColor->XAxiscolor[0] = 255;
    axisColor->XAxiscolor[1] = 0;
    axisColor->XAxiscolor[2] = 0;
    axisColor->YAxiscolor[0] = 0;
    axisColor->YAxiscolor[1] = 255;
    axisColor->YAxiscolor[2] = 0;
    axisColor->ZAxiscolor[0] = 0;
    axisColor->ZAxiscolor[1] = 0;
    axisColor->ZAxiscolor[2] = 255;
    axisColor->sameColor = false;
    axisColor->complementaryColor = true;
    shapePopulationBase->m_axisColor.push_back(axisColor);

    /// Test 2.
    /// - same color for positive and negative axis
    /// - X: Red Y: Green Z: Blue
    axisColor = new axisColorStruct;
    axisColor->XAxiscolor[0] = 255;
    axisColor->XAxiscolor[1] = 0;
    axisColor->XAxiscolor[2] = 0;
    axisColor->YAxiscolor[0] = 0;
    axisColor->YAxiscolor[1] = 255;
    axisColor->YAxiscolor[2] = 0;
    axisColor->ZAxiscolor[0] = 0;
    axisColor->ZAxiscolor[1] = 0;
    axisColor->ZAxiscolor[2] = 255;
    axisColor->sameColor = true;
    axisColor->complementaryColor = false;
    shapePopulationBase->m_axisColor.push_back(axisColor);

    /// Test 3.
    /// - same color for positive and negative axis
    /// - X: Red Y: Green Z: Yellow
    axisColor = new axisColorStruct;
    axisColor->XAxiscolor[0] = 255;
    axisColor->XAxiscolor[1] = 0;
    axisColor->XAxiscolor[2] = 0;
    axisColor->YAxiscolor[0] = 0;
    axisColor->YAxiscolor[1] = 255;
    axisColor->YAxiscolor[2] = 0;
    axisColor->ZAxiscolor[0] = 255;
    axisColor->ZAxiscolor[1] = 255;
    axisColor->ZAxiscolor[2] = 0;
    axisColor->sameColor = true;
    axisColor->complementaryColor = false;
    shapePopulationBase->m_axisColor.push_back(axisColor);

    /// Test 4.
    /// - complementary color for positive and negative axis
    /// - X: Red Y: Orange Z: Yellow
    axisColor = new axisColorStruct;
    axisColor->XAxiscolor[0] = 255;
    axisColor->XAxiscolor[1] = 0;
    axisColor->XAxiscolor[2] = 0;
    axisColor->YAxiscolor[0] = 255;
    axisColor->YAxiscolor[1] = 127;
    axisColor->YAxiscolor[2] = 0;
    axisColor->ZAxiscolor[0] = 255;
    axisColor->ZAxiscolor[1] = 255;
    axisColor->ZAxiscolor[2] = 0;
    axisColor->complementaryColor = true;
    axisColor->sameColor = false;
    shapePopulationBase->m_axisColor.push_back(axisColor);

    std::vector< const char* > colormap;
    colormap.push_back("ColorByDirection_colorByDefault");
    colormap.push_back("ColorByDirection_sameColor_RedGreenBlue");
    colormap.push_back("ColorByDirection_sameColor_RedGreenYellow");
    colormap.push_back("ColorByDirection_complementaryColor_RedOrangeYellow");


    // Call of the function that must be test
    shapePopulationBase->UpdateColorMapByDirection("PointToPointVector",0);

    // Test if the result obtained is correct:
    vtkPolyData* polyData1 = shapePopulationBase->m_meshList[0]->GetPolyData();
    vtkIdType numPts1 = polyData1->GetNumberOfPoints();

    // Recover of data to make a comparison
    vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
    meshReader->SetFileName(filenameExpectedResult.c_str());
    meshReader->Update();
    vtkPolyData* polyData2 = meshReader->GetOutput();
    vtkIdType numPts2 = polyData2->GetNumberOfPoints();

    if( numPts1 != numPts2 ) return 1;

    for( unsigned int i = 0; i < shapePopulationBase->m_selectedIndex.size(); i++)
    {
        polyData1 = shapePopulationBase->m_meshList[shapePopulationBase->m_selectedIndex[i]]->GetPolyData();
        vtkDataArray* map1 = polyData1->GetPointData()->GetArray(strs_dir.str().c_str());
        int nb1 = polyData1->GetPointData()->GetArray(strs_dir.str().c_str())->GetNumberOfComponents();

        vtkDataArray* map2 = polyData2->GetPointData()->GetArray(colormap[i]);
        int nb2 = polyData2->GetPointData()->GetArray(colormap[i])->GetNumberOfComponents();

        if(nb1 != nb2) return 1;

        double p[3];
        double s[3];
        for( vtkIdType k = 0; k < numPts2; ++k )
        {
            map1->GetTuple(k,p);
            map2->GetTuple(k,s);

            for(int j = 0; j < 3; j++)
            {
                if(fabs(p[j] - s[j]) > 0.00001)
                {
                    return 1;
                }
            }
        }
    }

    /// TEST BY CHANGING THE VALUE OF MAXIMUM OF MAGNITUDE ///

    shapePopulationBase->m_magnitude.clear();

    /// Test 5.
    /// - color by default
    /// - maximum of magnitude : 8
    magnitude = new magnitudStruct;
    magnitude->min = commonRange[0];
    magnitude->max = 8;
    shapePopulationBase->m_magnitude.push_back(magnitude);

    /// Test 6. Third Color map:
    /// - color by default
    /// - maximum of magnitude : 3
    magnitude = new magnitudStruct;
    magnitude->min = commonRange[0];
    magnitude->max = 3;
    shapePopulationBase->m_magnitude.push_back(magnitude);

    colormap.clear();
    colormap.push_back("ColorByDirection_magnitude8");
    colormap.push_back("ColorByDirection_magnitude3");

    for(unsigned int i = 0; i < shapePopulationBase->m_magnitude.size(); i++)
    {
        // Call of the function that must be test
        shapePopulationBase->UpdateColorMapByDirection("PointToPointVector",i);

        // Test if the result obtained is correct:
        polyData1 = shapePopulationBase->m_meshList[3]->GetPolyData();
        vtkDataArray* map1 = polyData1->GetPointData()->GetArray(strs_dir.str().c_str());
        int nb1 = polyData1->GetPointData()->GetArray(strs_dir.str().c_str())->GetNumberOfComponents();

        // Recover of data to make a comparison
        vtkDataArray* map2 = polyData2->GetPointData()->GetArray(colormap[i]);
        int nb2 = polyData2->GetPointData()->GetArray(colormap[i])->GetNumberOfComponents();

        if(nb1 != nb2) return 1;

        double p[3];
        double s[3];
        for( vtkIdType k = 0; k < numPts1; ++k )
        {
            map1->GetTuple(k,p);
            map2->GetTuple(k,s);

            for(int j = 0; j < 3; j++)
            {
                if(fabs(p[j] - s[j]) > 0.00001)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

#include "testCreationSphere.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testCreationSphereActor(std::string filename)
{
    int nbTest = 2;

    // Creation of sphere with color by direction
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    shapePopulationBase->m_axisColor.clear();
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
    /// - X: Purple Y: Yellow Z: Brown
    axisColor = new axisColorStruct;
    axisColor->XAxiscolor[0] = 127;
    axisColor->XAxiscolor[1] = 0;
    axisColor->XAxiscolor[2] = 127;
    axisColor->YAxiscolor[0] = 255;
    axisColor->YAxiscolor[1] = 255;
    axisColor->YAxiscolor[2] = 0;
    axisColor->ZAxiscolor[0] = 153;
    axisColor->ZAxiscolor[1] = 102;
    axisColor->ZAxiscolor[2] = 51;
    axisColor->sameColor = true;
    axisColor->complementaryColor = false;
    shapePopulationBase->m_axisColor.push_back(axisColor);

    std::vector< const char* > colormap;
    colormap.push_back("ColorByDirection_colorByDefault");
    colormap.push_back("ColorByDirection_sameColor_PurpleYellowBrown");

    // Recover of data to make a comparison
    vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
    meshReader->SetFileName(filename.c_str());
    meshReader->Update();
    vtkSmartPointer<vtkPolyData> SpherePolyData = vtkSmartPointer<vtkPolyData>::New();
    SpherePolyData = meshReader->GetOutput();

    for( int i = 0; i < nbTest; i++)
    {
        shapePopulationBase->m_selectedIndex.clear();
        shapePopulationBase->m_selectedIndex.push_back(i);

        // Call of the function that must be test
        vtkSmartPointer<vtkActor> actorSphere;
        actorSphere.TakeReference(shapePopulationBase->creationSphereActor());

        // Test if the result obtained is correct
        vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
        polyData = (vtkPolyData* )actorSphere->GetMapper()->GetInput();
        vtkIdType numPts1 = polyData->GetNumberOfPoints();
        vtkDoubleArray* map1 = vtkDoubleArray::New();
        map1 = (vtkDoubleArray*)polyData->GetPointData()->GetArray("ColorByDirection");
        int nb1 = polyData->GetPointData()->GetArray("ColorByDirection")->GetNumberOfComponents();


        vtkIdType numPts2 = SpherePolyData->GetNumberOfPoints();
        vtkDoubleArray* map2 = vtkDoubleArray::New();
        map2 = (vtkDoubleArray*)SpherePolyData->GetPointData()->GetArray(colormap[i]);
        int nb2 = SpherePolyData->GetPointData()->GetArray(colormap[i])->GetNumberOfComponents();

        if(numPts1 != numPts2) return 1;

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


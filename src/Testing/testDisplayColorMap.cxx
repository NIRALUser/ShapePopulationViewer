#include "testDisplayColorMap.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testDisplayColorMap(std::string filename, std::string filenameExpectedResult)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    const char* cmap = "PointToPointVector";
    std::ostringstream strs_mag;
    strs_mag.str("");
    strs_mag.clear();
    strs_mag << cmap << "_mag" << std::endl;

    std::ostringstream strs_dir;
    strs_dir.str("");
    strs_dir.clear();
    strs_dir << cmap << "_ColorByDirection" << std::endl;

    int nbMesh = 4;
    shapePopulationBase->m_windowsList.clear();
    shapePopulationBase->m_selectedIndex.clear();
    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayVectors.push_back(false);
        shapePopulationBase->m_displayColorMapByMagnitude.push_back(true);
        shapePopulationBase->m_displayVectorsByMagnitude.push_back(false);
        shapePopulationBase->m_displayColorMapByDirection.push_back(false);
        shapePopulationBase->m_displayVectorsByDirection.push_back(false);
        shapePopulationBase->m_selectedIndex.push_back(i);
        shapePopulationBase->CreateNewWindow(filename, /* testing = */ true);
        shapePopulationBase->m_meshList[i]->GetPolyData()->GetPointData()->SetActiveScalars(strs_mag.str().c_str());

        // UPDATE THE COLOR MAP BY DIRECTION
        // magnitude
        double *commonRange = shapePopulationBase->computeCommonRange(strs_mag.str().c_str(),shapePopulationBase->m_selectedIndex);
        magnitudStruct* magnitude = new magnitudStruct;
        magnitude->min = commonRange[0];
        magnitude->max = commonRange[1];
        shapePopulationBase->m_magnitude.push_back(magnitude);

        // color of axis
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

        shapePopulationBase->UpdateColorMapByDirection(cmap,i);

    }
    shapePopulationBase->initializationAllWidgets();


    /// Initial state.
    /// Selected Meshes: color map by magnitude

    bool check = checkingColorMap(nbMesh,
                                  filenameExpectedResult,
                                  strs_mag.str().c_str(),
                                  strs_mag.str().c_str(),
                                  shapePopulationBase->m_meshList,
                                  shapePopulationBase->m_selectedIndex);
    if(check) return check;

    /// TEST 1.
    /// Selected Meshes: color map by direction

    // Call of the function that must be test
    shapePopulationBase->displayColorMapByDirection(true);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_dir.str().c_str(),
                             strs_dir.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return check;

    /// TEST 2.
    /// Selected Meshes: color map by magnitude
    /// Unselected Meshes: color map by direction

    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
    }

    // Call of the function that must be test
    shapePopulationBase->displayColorMapByMagnitude(true);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_mag.str().c_str(),
                             strs_dir.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);    if(check) return check;


    /// TEST 3.
    /// Selected Meshes: color map by magnitude

    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.push_back(i);
    }
    // Call of the function that must be test
    shapePopulationBase->displayColorMapByMagnitude(true);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_mag.str().c_str(),
                             strs_mag.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return check;

    /// TEST 4.
    /// Selected Meshes: color map by direction
    /// Unselected Meshes: color map by magnitude

    for(int i = 1; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
    }

    // Call of the function that must be test
    shapePopulationBase->displayColorMapByDirection(true);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_dir.str().c_str(),
                             strs_mag.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return check;

    return 0;

}


bool TestShapePopulationBase::checkingColorMap(int nbMesh, std::string filenameExpectedResult, const char* cmap_selectedMesches, const char* cmap_unSelectedMesches, std::vector<ShapePopulationData *> m_meshList, std::vector< unsigned int > m_selectedIndex)
{
    for(int i = 0; i < nbMesh; i++)
    {
        const char* cmap = m_meshList[i]->GetPolyData()->GetPointData()->GetScalars()->GetName();

        vtkPolyData* polyData1 = m_meshList[i]->GetPolyData();
        vtkIdType numPts1 = polyData1->GetNumberOfPoints();
        vtkDataArray* map1 = polyData1->GetPointData()->GetArray(cmap);
        int nb1 = polyData1->GetPointData()->GetArray(cmap)->GetNumberOfComponents();

        vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
        vtkIdType numPts2;
        vtkDataArray* map2 = 0;
        int nb2;

        if( (std::find(m_selectedIndex.begin(), m_selectedIndex.end(), i)) != (m_selectedIndex.end()) )
        {
            if(strcmp(cmap,cmap_selectedMesches)) return 1;

            // Recover of data to make a comparison
            meshReader->SetFileName(filenameExpectedResult.c_str());
            meshReader->Update();
            vtkPolyData* polyData2 = meshReader->GetOutput();
            numPts2 = polyData2->GetNumberOfPoints();
            map2 = polyData2->GetPointData()->GetArray(cmap_selectedMesches);
            nb2 = polyData2->GetPointData()->GetArray(cmap_selectedMesches)->GetNumberOfComponents();
        }
        else
        {
            if(strcmp(cmap,cmap_unSelectedMesches)) return 1;

            // Recover of data to make a comparison
            meshReader->SetFileName(filenameExpectedResult.c_str());
            meshReader->Update();
            vtkPolyData* polyData2 = meshReader->GetOutput();
            numPts2 = polyData2->GetNumberOfPoints();
            map2 = polyData2->GetPointData()->GetArray(cmap_unSelectedMesches);
            nb2 = polyData2->GetPointData()->GetArray(cmap_unSelectedMesches)->GetNumberOfComponents();
        }
        if( numPts1 != numPts2 ) return 1;

        if(nb1 != nb2) return 1;

        double p[3];
        double s[3];
        for( vtkIdType k = 0; k < numPts1; ++k )
        {
            map1->GetTuple(k,p);
            map2->GetTuple(k,s);
            for(int j = 0; j < nb1; ++j )
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



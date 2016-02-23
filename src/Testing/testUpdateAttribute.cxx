#include "testUpdateAttribute.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testUpdateAttribute(std::string filename, std::string filenameExpectedResult)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    const char* cmap_selectedMeshes = "Normals";
    std::ostringstream strs_selectedMeshes;
    strs_selectedMeshes.str("");
    strs_selectedMeshes.clear();
    strs_selectedMeshes << cmap_selectedMeshes << "_mag" << std::endl;

    int nbMesh = 4;
    shapePopulationBase->initializationAllWidgets();
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
        shapePopulationBase->CreateNewWindow(filename);
        shapePopulationBase->m_meshList[i]->GetPolyData()->GetPointData()->SetActiveScalars(strs_selectedMeshes.str().c_str());

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
    }

    // UPDATE THE COLOR MAP BY DIRECTION
    shapePopulationBase->computeCommonAttributes();
    for(unsigned int i = 0; i < shapePopulationBase->m_commonAttributes.size(); i++)
    {
        int dim = shapePopulationBase->m_meshList[shapePopulationBase->m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars(shapePopulationBase->m_commonAttributes[i].c_str())->GetNumberOfComponents();
        // magnitude

        if(dim == 1)
        {
            double *commonRange = shapePopulationBase->computeCommonRange(shapePopulationBase->m_commonAttributes[i].c_str(),shapePopulationBase->m_selectedIndex);
            magnitudStruct* magnitude = new magnitudStruct;
            magnitude->min = commonRange[0];
            magnitude->max = commonRange[1];
            shapePopulationBase->m_magnitude.push_back(magnitude);
        }


        if(dim == 3)
        {
            std::ostringstream strs;
            strs.str("");
            strs.clear();
            strs << shapePopulationBase->m_commonAttributes[i] << "_mag" << std::endl;
            double *commonRange = shapePopulationBase->computeCommonRange(strs.str().c_str(),shapePopulationBase->m_selectedIndex);
            magnitudStruct* magnitude = new magnitudStruct;
            magnitude->min = commonRange[0];
            magnitude->max = commonRange[1];
            shapePopulationBase->m_magnitude.push_back(magnitude);
            shapePopulationBase->UpdateColorMapByDirection(shapePopulationBase->m_commonAttributes[i].c_str(),i);
        }
    }

    /// Initial state :
    /// Attribute: Normals (3 dimension)
    /// Color map: Magnitude

    const char *cmap_unSelectedMeshes;
    cmap_unSelectedMeshes = cmap_selectedMeshes;
    std::ostringstream strs_unSelectedMeshes;
    strs_unSelectedMeshes.str("");
    strs_unSelectedMeshes.clear();
    strs_unSelectedMeshes << cmap_unSelectedMeshes;

    // Test if the result obtained is correct:
    bool check = checkingColorMap(nbMesh,
                                  filenameExpectedResult,
                                  strs_selectedMeshes.str().c_str(),
                                  strs_unSelectedMeshes.str().c_str(),
                                  shapePopulationBase->m_meshList,
                                  shapePopulationBase->m_selectedIndex);
    if(check) return 1;

    /// Test 1.
    /// Attribute: PointToPointVector (3 dimension)
    /// Color map: Magnitude
    cmap_selectedMeshes = "PointToPointVector";
    strs_selectedMeshes.str("");
    strs_selectedMeshes.clear();
    strs_selectedMeshes << cmap_selectedMeshes << "_mag" << std::endl;

    cmap_unSelectedMeshes = cmap_selectedMeshes;
    strs_unSelectedMeshes.str("");
    strs_unSelectedMeshes.clear();
    strs_unSelectedMeshes << cmap_unSelectedMeshes;

    // Call of the function that must be test
    shapePopulationBase->UpdateAttribute(cmap_selectedMeshes,shapePopulationBase->m_selectedIndex);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_selectedMeshes.str().c_str(),
                             strs_unSelectedMeshes.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return 1;

    /// Test 2.
    /// Attribute: MagNormVector (3 dimension)
    /// Color map: Direction
    cmap_selectedMeshes = "MagNormVector";
    strs_selectedMeshes.str("");
    strs_selectedMeshes.clear();
    strs_selectedMeshes << cmap_selectedMeshes << "_ColorByDirection" << std::endl;

    cmap_unSelectedMeshes = cmap_selectedMeshes;
    strs_unSelectedMeshes.str("");
    strs_unSelectedMeshes.clear();
    strs_unSelectedMeshes << cmap_unSelectedMeshes;

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByMagnitude[i] = false;
        shapePopulationBase->m_displayColorMapByDirection[i] = true;
    }

    // Call of the function that must be test
    shapePopulationBase->UpdateAttribute(cmap_selectedMeshes,shapePopulationBase->m_selectedIndex);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_selectedMeshes.str().c_str(),
                             strs_unSelectedMeshes.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return 1;

    /// Test 3.
    /// Attribute: PointToPointAlongZ (1 dimension)
    /// Color map: Magnitude

    cmap_selectedMeshes = "PointToPointAlongZ";
    strs_selectedMeshes.str("");
    strs_selectedMeshes.clear();
    strs_selectedMeshes << cmap_selectedMeshes;

    cmap_unSelectedMeshes = cmap_selectedMeshes;
    strs_unSelectedMeshes.str("");
    strs_unSelectedMeshes.clear();
    strs_unSelectedMeshes << cmap_unSelectedMeshes;

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByMagnitude[i] = true;
        shapePopulationBase->m_displayColorMapByDirection[i] = false;
    }

    // Call of the function that must be test
    shapePopulationBase->UpdateAttribute(cmap_selectedMeshes,shapePopulationBase->m_selectedIndex);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_selectedMeshes.str().c_str(),
                             strs_unSelectedMeshes.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return 1;

    /// Test 4.
    /// Selected meshes:
    ///     - Attribute: PointToPointVector (3 dimension)
    ///     - Color map: Direction
    /// Unselected meshes:
    ///     - Attribute: PointToPointAlongZ (1 dimension)
    ///     - Color map: Magnitude

    cmap_unSelectedMeshes = cmap_selectedMeshes;
    strs_unSelectedMeshes.str("");
    strs_unSelectedMeshes.clear();
    strs_unSelectedMeshes << cmap_unSelectedMeshes;

    cmap_selectedMeshes = "PointToPointVector";
    strs_selectedMeshes.str("");
    strs_selectedMeshes.clear();
    strs_selectedMeshes << cmap_selectedMeshes << "_ColorByDirection" << std::endl;

    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
        shapePopulationBase->m_displayColorMapByMagnitude[shapePopulationBase->m_selectedIndex[i]] = false;
        shapePopulationBase->m_displayColorMapByDirection[shapePopulationBase->m_selectedIndex[i]] = true;
    }

    // Call of the function that must be test
    shapePopulationBase->UpdateAttribute(cmap_selectedMeshes,shapePopulationBase->m_selectedIndex);

    // Test if the result obtained is correct:
    check = checkingColorMap(nbMesh,
                             filenameExpectedResult,
                             strs_selectedMeshes.str().c_str(),
                             strs_unSelectedMeshes.str().c_str(),
                             shapePopulationBase->m_meshList,
                             shapePopulationBase->m_selectedIndex);
    if(check) return 1;

    return 0;

}

bool TestShapePopulationBase::checkingColorMap(int nbMesh, std::string filenameExpectedResult, const char* cmap_selectedMesches, const char* cmap_unSelectedMesches, std::vector<ShapePopulationData *> m_meshList, std::vector< unsigned int > m_selectedIndex)
{
    for(int i = 0; i < nbMesh; i++)
    {
        const char* cmap = m_meshList[i]->GetPolyData()->GetPointData()->GetScalars()->GetName();

        vtkSmartPointer<vtkPolyData> polyData1 = vtkSmartPointer<vtkPolyData>::New();
        polyData1 = m_meshList[i]->GetPolyData();
        vtkIdType numPts1 = polyData1->GetNumberOfPoints();
        vtkDoubleArray* map1 = vtkDoubleArray::New();
        map1 = (vtkDoubleArray*)polyData1->GetPointData()->GetArray(cmap);
        int nb1 = polyData1->GetPointData()->GetArray(cmap)->GetNumberOfComponents();

        vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
        vtkSmartPointer<vtkPolyData> polyData2 = vtkSmartPointer<vtkPolyData>::New();
        vtkIdType numPts2;
        vtkDoubleArray* map2 = vtkDoubleArray::New();
        int nb2;

        if( (std::find(m_selectedIndex.begin(), m_selectedIndex.end(), i)) != (m_selectedIndex.end()) )
        {
            if(strcmp(cmap,cmap_selectedMesches)) return 1;

            // Recover of data to make a comparison
            meshReader->SetFileName(filenameExpectedResult.c_str());
            meshReader->Update();
            polyData2 = meshReader->GetOutput();
            numPts2 = polyData2->GetNumberOfPoints();
            map2 = (vtkDoubleArray*)polyData2->GetPointData()->GetArray(cmap_selectedMesches);
            nb2 = polyData2->GetPointData()->GetArray(cmap_selectedMesches)->GetNumberOfComponents();
        }
        else
        {
            if(strcmp(cmap,cmap_unSelectedMesches)) return 1;

            // Recover of data to make a comparison
            meshReader->SetFileName(filenameExpectedResult.c_str());
            meshReader->Update();
            polyData2 = meshReader->GetOutput();
            numPts2 = polyData2->GetNumberOfPoints();
            map2 = (vtkDoubleArray*)polyData2->GetPointData()->GetArray(cmap_unSelectedMesches);
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



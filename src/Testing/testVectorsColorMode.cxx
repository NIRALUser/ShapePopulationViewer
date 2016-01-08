#include "testVectorsColorMode.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testVectorsColorMode(std::string filename)
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
        shapePopulationBase->CreateNewWindow(filename);
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
    shapePopulationBase->computeCommonAttributes();

    /// Test 1.
    /// Color map: Magnitude
    /// Color of vectors: Magnitude
    /// Mode of vectors: SetColorModeToColorByVector

    shapePopulationBase->displayColorMapByMagnitude(true);
    shapePopulationBase->displayVectors(true);
    shapePopulationBase->displayVectorsByMagnitude(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[shapePopulationBase->m_selectedIndex[i]];
        int colorMode = glyph->GetColorMode();
        if(colorMode != 2) return 1;
    }

    /// Test 2.
    /// Color map: Direction
    /// Color of vectors: Direction
    /// Mode of vectors: SetColorModeToColorByScalar

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByMagnitude[shapePopulationBase->m_selectedIndex[i]] = false;
    }
    shapePopulationBase->displayColorMapByDirection(true);

    shapePopulationBase->displayVectorsByDirection(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[shapePopulationBase->m_selectedIndex[i]];
        int colorMode = glyph->GetColorMode();
        if(colorMode != 1) return 1;
    }

    /// Test 3.
    /// Color map: Direction
    /// Color of vectors: Magnitude
    /// Mode of vectors: SetColorModeToColorByVector

    shapePopulationBase->displayVectorsByMagnitude(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[shapePopulationBase->m_selectedIndex[i]];
        int colorMode = glyph->GetColorMode();
        if(colorMode != 2) return 1;
    }

    /// Test 4.
    /// Color map: Magnitude
    /// Color of vectors: Magnitude
    /// Mode of vectors: SetColorModeToColorByVector

    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_displayColorMapByDirection[shapePopulationBase->m_selectedIndex[i]] = false;
    }
    shapePopulationBase->displayColorMapByMagnitude(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[shapePopulationBase->m_selectedIndex[i]];
        int colorMode = glyph->GetColorMode();
        if(colorMode != 2) return 1;
    }

    /// Test 5.
    /// Color map: Magnitude
    /// Color of vectors: Direction
    /// Mode of vectors: SetColorModeToColorByScalar

    shapePopulationBase->displayVectorsByDirection(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[shapePopulationBase->m_selectedIndex[i]];
        int colorMode = glyph->GetColorMode();
        if(colorMode != 1) return 1;
    }

    /// Test 5.
    /// Color map: Magnitude
    /// Selected Meshes:
    ///     - Color of vectors: Magnitude
    ///     - Mode of vectors: SetColorModeToColorByVector
    /// Unselected Meshes:
    ///     - Color of vectors: Direction
    ///     - Mode of vectors: SetColorModeToColorByScalar

    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
    }

    shapePopulationBase->displayVectorsByMagnitude(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[i];
        int colorMode = glyph->GetColorMode();
        if( (std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)) != (shapePopulationBase->m_selectedIndex.end()) )
        {
            if(colorMode != 2) return 1;
        }
        else
        {
            if(colorMode != 1) return 1;
        }
    }

    /// Test 6.
    /// Color map: Direction
    /// Color of vectors: Magnitude
    /// Mode of vectors: SetColorModeToColorByVector


    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.push_back(i);
    }

    shapePopulationBase->displayColorMapByDirection(true);
    shapePopulationBase->displayVectorsByMagnitude(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[shapePopulationBase->m_selectedIndex[i]];
        int colorMode = glyph->GetColorMode();
        if(colorMode != 2) return 1;
    }

    /// Test 7.
    /// Color map: Direction
    /// Selected Meshes:
    ///     - Color of vectors: Direction
    ///     - Mode of vectors: SetColorModeToColorByScalar
    /// Unselected Meshes:
    ///     - Color of vectors: Magnitude
    ///     - Mode of vectors: SetColorModeToColorByVector

    for(int i = 0; i < nbMesh; i = i + 2)
    {
        shapePopulationBase->m_selectedIndex.erase((std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)));
    }

    shapePopulationBase->displayVectorsByDirection(true);

    // Test if the result obtained is correct:
    for(int i = 0; i < nbMesh; i++)
    {
        vtkSmartPointer<vtkGlyph3D> glyph = shapePopulationBase->m_glyphList[i];
        int colorMode = glyph->GetColorMode();
        if( (std::find(shapePopulationBase->m_selectedIndex.begin(), shapePopulationBase->m_selectedIndex.end(), i)) != (shapePopulationBase->m_selectedIndex.end()) )
        {
            if(colorMode != 1) return 1;
        }
        else
        {
            if(colorMode != 2) return 1;
        }
    }

    return 0;

}





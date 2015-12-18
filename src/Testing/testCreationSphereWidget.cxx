
#include "testCreationSphereWidget.h"
#include "QSharedPointer.h"
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testCreationSphereWidget(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );

    int nbTest = 2;
    shapePopulationBase->m_windowsList.clear();
    for(int i = 0; i < nbTest; i++)
    {
        shapePopulationBase->CreateNewWindow(filename);
    }

    shapePopulationBase->initializationAllWidgets();
    shapePopulationBase->m_displaySphere = true;

    // Label of color of axis in green
    shapePopulationBase->m_labelColor[0] = 0;
    shapePopulationBase->m_labelColor[1] = 1;
    shapePopulationBase->m_labelColor[2] = 0;

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

    for(unsigned int i = 0; i < shapePopulationBase->m_axisColor.size(); i++)
    {
        shapePopulationBase->m_selectedIndex.clear();
        shapePopulationBase->m_selectedIndex.push_back(i);

        // Call of the function that must be test
        shapePopulationBase->creationSphereWidget(i);

        // Test if the result obtained is correct:
              // AXIS
        vtkOrientationMarkerWidget* widgetAxisByDirection = vtkOrientationMarkerWidget::New();
        widgetAxisByDirection = shapePopulationBase->m_widgetAxisByDirection[i];
        vtkProp* prop = widgetAxisByDirection->GetOrientationMarker();
        vtkAxesActor* axis = vtkAxesActor::New();
        axis = (vtkAxesActor*)prop;

        double *Xcolor = axis->GetXAxisShaftProperty()->GetColor();
        if(shapePopulationBase->m_axisColor[i]->XAxiscolor[0] != (int)(Xcolor[0]*255) || shapePopulationBase->m_axisColor[i]->XAxiscolor[1] != (int)(Xcolor[1]*255) || shapePopulationBase->m_axisColor[i]->XAxiscolor[2] != (int)(Xcolor[2]*255))
        {
            return 1;
        }
        Xcolor = axis->GetXAxisTipProperty()->GetColor();
        if(shapePopulationBase->m_axisColor[i]->XAxiscolor[0] != (int)(Xcolor[0]*255) || shapePopulationBase->m_axisColor[i]->XAxiscolor[1] != (int)(Xcolor[1]*255) || shapePopulationBase->m_axisColor[i]->XAxiscolor[2] != (int)(Xcolor[2]*255))
        {
            return 1;
        }
        double *Ycolor = axis->GetYAxisShaftProperty()->GetColor();
        if(shapePopulationBase->m_axisColor[i]->YAxiscolor[0] != (int)(Ycolor[0]*255) || shapePopulationBase->m_axisColor[i]->YAxiscolor[1] != (int)(Ycolor[1]*255) || shapePopulationBase->m_axisColor[i]->YAxiscolor[2] != (int)(Ycolor[2]*255))
        {
            return 1;
        }
        Ycolor = axis->GetYAxisTipProperty()->GetColor();
        if(shapePopulationBase->m_axisColor[i]->YAxiscolor[0] != (int)(Ycolor[0]*255) || shapePopulationBase->m_axisColor[i]->YAxiscolor[1] != (int)(Ycolor[1]*255) || shapePopulationBase->m_axisColor[i]->YAxiscolor[2] != (int)(Ycolor[2]*255))
        {
            return 1;
        }
        double *Zcolor = axis->GetZAxisShaftProperty()->GetColor();
        if(shapePopulationBase->m_axisColor[i]->ZAxiscolor[0] != (int)(Zcolor[0]*255) || shapePopulationBase->m_axisColor[i]->ZAxiscolor[1] != (int)(Zcolor[1]*255) || shapePopulationBase->m_axisColor[i]->ZAxiscolor[2] != (int)(Zcolor[2]*255))
        {
            return 1;
        }
        Zcolor = axis->GetZAxisTipProperty()->GetColor();
        if(shapePopulationBase->m_axisColor[i]->ZAxiscolor[0] != (int)(Zcolor[0]*255) || shapePopulationBase->m_axisColor[i]->ZAxiscolor[1] != (int)(Zcolor[1]*255) || shapePopulationBase->m_axisColor[i]->ZAxiscolor[2] != (int)(Zcolor[2]*255))
        {
            return 1;
        }

            // SPHERE
        vtkOrientationMarkerWidget* widgetSphere = vtkOrientationMarkerWidget::New();
        widgetSphere = shapePopulationBase->m_widgetSphere[i];
        vtkProp* propSphere = widgetSphere->GetOrientationMarker();
        vtkActor* actorSphere = vtkActor::New();
        actorSphere = (vtkActor*)propSphere;

        // Test if the result obtained is correct
        vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
        polyData = (vtkPolyData* )actorSphere->GetMapper()->GetInput();
        vtkIdType numPts1 = polyData->GetNumberOfPoints();
        vtkDoubleArray* map1 = vtkDoubleArray::New();
        map1 = (vtkDoubleArray*)polyData->GetPointData()->GetArray("ColorByDirection");
        int nb1 = polyData->GetPointData()->GetArray("ColorByDirection")->GetNumberOfComponents();

        vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
        meshReader->SetFileName(filename.c_str());
        meshReader->Update();
        vtkSmartPointer<vtkPolyData> SpherePolyData = vtkSmartPointer<vtkPolyData>::New();
        SpherePolyData = meshReader->GetOutput();
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

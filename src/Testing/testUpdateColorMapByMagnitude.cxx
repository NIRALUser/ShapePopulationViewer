#include "testUpdateColorMapByMagnitude.h"
#include <QSharedPointer>
#include "ShapePopulationQT.h"

TestShapePopulationBase::TestShapePopulationBase()
{

}

bool TestShapePopulationBase::testUpdateColorMapByMagnitude(std::string filename)
{
    QSharedPointer<ShapePopulationBase> shapePopulationBase = QSharedPointer<ShapePopulationBase>( new ShapePopulationBase );
    const char * cmap = "AbsoluteMagNormDirDistance";
    int nbMesh = 2;
    shapePopulationBase->m_windowsList.clear();
    for(int i = 0; i < nbMesh; i++)
    {
        shapePopulationBase->m_selectedIndex.push_back(i);
        shapePopulationBase->CreateNewWindow(filename, /* testing = */ true);
        shapePopulationBase->m_meshList[i]->GetPolyData()->GetPointData()->SetActiveScalars(cmap);
    }

    // Range of the color map by magnitude
    shapePopulationBase->computeCommonAttributes();
    int nbColorPoint = 3;
    double RGB[3][3] = {{0,1,0},
                        {1,1,0},
                        {1,0,0}};

    for(unsigned int i = 0; i < shapePopulationBase->m_commonAttributes.size(); i++)
    {
        int dim = shapePopulationBase->m_meshList[shapePopulationBase->m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars(shapePopulationBase->m_commonAttributes[i].c_str())->GetNumberOfComponents();
        if(dim == 1)
        {
            double *commonRange = shapePopulationBase->computeCommonRange(shapePopulationBase->m_commonAttributes[i].c_str(),shapePopulationBase->m_selectedIndex);
            colorBarStruct* colorBar = new colorBarStruct;
            colorBar->range[0] = commonRange[0];
            colorBar->range[1] = commonRange[1];
            shapePopulationBase->m_colorBarList.push_back(colorBar);
        }
        else if(dim == 3)
        {
            std::ostringstream strs;
            strs.str("");
            strs.clear();
            strs << shapePopulationBase->m_commonAttributes[i] << "_mag" << std::endl;
            double *commonRange = shapePopulationBase->computeCommonRange(strs.str().c_str(),shapePopulationBase->m_selectedIndex);
            colorBarStruct* colorBar = new colorBarStruct;
            colorBar->range[0] = commonRange[0];
            colorBar->range[1] = commonRange[1];
            shapePopulationBase->m_colorBarList.push_back(colorBar);

        }
        for(int k = 0; k < nbColorPoint; k++)
        {
            colorPointStruct colorPoint;
            colorPoint.pos = ( round( (k * ( 1.0/(nbColorPoint - 1.0 ) ) ) * 10.0) ) / 10.0;
            colorPoint.r = RGB[k][0];
            colorPoint.g = RGB[k][1];
            colorPoint.b = RGB[k][2];
            shapePopulationBase->m_colorBarList[i]->colorPointList.push_back(colorPoint);
        }
    }
    shapePopulationBase->m_usedColorBar = shapePopulationBase->m_colorBarList[1];

    // Call of the function that must be test
    shapePopulationBase->UpdateColorMapByMagnitude(shapePopulationBase->m_selectedIndex);

    for(int i = 0; i < nbMesh; i++)
    {
        vtkActorCollection * actors = shapePopulationBase->m_windowsList[shapePopulationBase->m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
        actors->InitTraversal();
        vtkMapper* mapper = actors->GetNextActor()->GetMapper();
        vtkScalarsToColors* scalarsToColors = mapper->GetLookupTable();
        vtkSmartPointer<vtkColorTransferFunction> DistanceMapTFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
        DistanceMapTFunc = (vtkColorTransferFunction *)scalarsToColors;
        double range = fabs(shapePopulationBase->m_usedColorBar->range[1] - shapePopulationBase->m_usedColorBar->range[0]);
        for (unsigned int j = 0; j < shapePopulationBase->m_usedColorBar->colorPointList.size(); j++)
        {
            double position = shapePopulationBase->m_usedColorBar->colorPointList[j].pos;
            double x = shapePopulationBase->m_usedColorBar->range[0] + range * position;
            double rgb[3];
            DistanceMapTFunc->GetColor(x, rgb);
            if(rgb[0] != RGB[j][0] || rgb[1] != RGB[j][1] || rgb[2] != RGB[j][2]) return 1;
        }
    }

    return 0;
}





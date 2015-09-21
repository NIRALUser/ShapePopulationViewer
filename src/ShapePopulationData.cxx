#include "ShapePopulationData.h"

static bool endswith(std::string file, std::string ext)
{
    int epos = file.length() - ext.length();
    if (epos < 0)
    {
        return false;
    }
    return file.rfind(ext) == (unsigned int)epos;
}

ShapePopulationData::ShapePopulationData()
{
    m_PolyData = vtkSmartPointer<vtkPolyData>::New();
}

vtkSmartPointer<vtkPolyData> ShapePopulationData::ReadPolyData(std::string a_filePath)
{
    if (endswith(a_filePath, ".vtp"))
    {
        vtkSmartPointer<vtkXMLPolyDataReader> meshReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
        meshReader->SetFileName(a_filePath.c_str());
        meshReader->Update();
        return meshReader->GetOutput();
    }
    else if (endswith(a_filePath, ".vtk"))
    {
        vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
        meshReader->SetFileName(a_filePath.c_str());
        meshReader->Update();
        return meshReader->GetOutput();
    }
    else
    {
        return NULL;
    }
}

void ShapePopulationData::AddColorByDirection(vtkSmartPointer<vtkPolyData> PolyData, const char * cmap)
{
    int numPts = PolyData->GetPoints()->GetNumberOfPoints();
    vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
    scalars->SetNumberOfComponents(3);
    scalars->SetNumberOfTuples(numPts);
    std::ostringstream strs;
    strs.str("");
    strs.clear();
    strs << cmap << "_ColorByDirection" << std::endl;
    scalars->SetName(strs.str().c_str());
    double RGB[3];
    
    vtkDataArray *vector;
    vector = PolyData->GetPointData()->GetScalars(cmap);
    
    // norm max
    std::vector<double> normVector;
    for( int i = 0; i < numPts; ++i )
        normVector.push_back(sqrt( vector->GetComponent(i,0)*vector->GetComponent(i,0) + vector->GetComponent(i,1)*vector->GetComponent(i,1) + vector->GetComponent(i,2)*vector->GetComponent(i,2) ));
    std::sort (normVector.begin(), normVector.end()); // using default comparison (operator <)
    double norm = normVector[numPts-1];
    
    // minimum and maximum
    double min[3];
    double max[3];
    for(int j = 0; j < 3; j++)
    {
        std::vector<double> vectXYZ;
        for( int i = 0; i < numPts; ++i ) {
            vectXYZ.push_back(vector->GetComponent(i,j)/norm);
        }
        std::sort (vectXYZ.begin(), vectXYZ.end()); // using default comparison (operator <):
        min[j] = vectXYZ[0];
        max[j] = vectXYZ[numPts-1];
    }
    
    // RGB scalar corresponding
    for( int i = 0; i < numPts; ++i )
    {
        float x = vector->GetComponent(i,0);
        float y = vector->GetComponent(i,1);
        float z = vector->GetComponent(i,2);
        
        if(max[0] != min[0]) RGB[0] = ((255/(max[0]-min[0]))*(x/norm)-(255*min[0])/(max[0]-min[0]));
        else if (max[0] != 0) RGB[0] = 255;
        else if (max[0] == 0) RGB[0] = 0;
        
        if(max[1] != min[1]) RGB[1] = ((255/(max[1]-min[1]))*(y/norm)-(255*min[1])/(max[1]-min[1]));
        else if (max[1] != 0) RGB[1] = 255;
        else if (max[1] == 0) RGB[1] = 0;
        
        
        if(max[2] != min[2]) RGB[2] = ((255/(max[2]-min[2]))*(z/norm)-(255*min[2])/(max[2]-min[2]));
        else if (max[2] != 0) RGB[2] = 255;
        else if (max[2] == 0) RGB[2] = 0;
        
        scalars->InsertTuple(i,RGB);
    }
    PolyData->GetPointData()->AddArray(scalars);
}

void ShapePopulationData::AddAbsoluteColorByDirection(vtkSmartPointer<vtkPolyData> PolyData, const char * cmap)
{
    int numPts = PolyData->GetPoints()->GetNumberOfPoints();
    vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
    scalars->SetNumberOfComponents(3);
    scalars->SetNumberOfTuples(numPts);
    std::ostringstream strs;
    strs.str("");
    strs.clear();
    strs << cmap << "_AbsoluteColorByDirection" << std::endl;
    scalars->SetName(strs.str().c_str());
    double RGB[3];
    
    vtkDataArray *vector;
    vector = PolyData->GetPointData()->GetScalars(cmap);
    
    // norm max
    std::vector<double> normVector;
    for( int i = 0; i < numPts; ++i )
        normVector.push_back(sqrt( vector->GetComponent(i,0)*vector->GetComponent(i,0) + vector->GetComponent(i,1)*vector->GetComponent(i,1) + vector->GetComponent(i,2)*vector->GetComponent(i,2) ));
    std::sort (normVector.begin(), normVector.end()); // using default comparison (operator <)
    double norm = normVector[numPts-1];
    
    // minimum and maximum
    double min[3];
    double max[3];
    for(int j = 0; j < 3; j++)
    {
        std::vector<double> vectXYZ;
        for( int i = 0; i < numPts; ++i ) {
            vectXYZ.push_back(fabs(vector->GetComponent(i,j))/norm);
        }
        std::sort (vectXYZ.begin(), vectXYZ.end()); // using default comparison (operator <)
        min[j] = vectXYZ[0];
        max[j] = vectXYZ[numPts-1];
    }
    // RGB scalar corresponding
    for( int i = 0; i < numPts; ++i )
    {
        float x = fabs(vector->GetComponent(i,0));
        float y = fabs(vector->GetComponent(i,1));
        float z = fabs(vector->GetComponent(i,2));
        
        if(max[0] != min[0]) RGB[0] = ((255/(max[0]-min[0]))*(x/norm)-(255*min[0])/(max[0]-min[0]));
        else if (max[0] != 0) RGB[0] = 255;
        else if (max[0] == 0) RGB[0] = 0;
        
        if(max[1] != min[1]) RGB[1] = ((255/(max[1]-min[1]))*(y/norm)-(255*min[1])/(max[1]-min[1]));
        else if (max[1] != 0) RGB[1] = 255;
        else if (max[1] == 0) RGB[1] = 0;
        
        
        if(max[2] != min[2]) RGB[2] = ((255/(max[2]-min[2]))*(z/norm)-(255*min[2])/(max[2]-min[2]));
        else if (max[2] != 0) RGB[2] = 255;
        else if (max[2] == 0) RGB[2] = 0;
        
        scalars->InsertTuple(i,RGB);
        
    }
    PolyData->GetPointData()->AddArray(scalars);
}

void ShapePopulationData::ReadMesh(std::string a_filePath)
{
    vtkSmartPointer<vtkPolyData> polyData = ReadPolyData(a_filePath);
    if(polyData == NULL) return;
    
    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
#if (VTK_MAJOR_VERSION < 6)
    normalGenerator->SetInput(polyData);
#else
    normalGenerator->SetInputData(polyData);
#endif
    normalGenerator->SplittingOff();
    normalGenerator->ComputePointNormalsOn();
    normalGenerator->ComputeCellNormalsOff();
    normalGenerator->Update();
    
    //Update the class members
    m_PolyData = normalGenerator->GetOutput();
    m_FilePath = a_filePath;
    size_t found = m_FilePath.find_last_of("/\\");
    m_FileDir = m_FilePath.substr(0,found);
    m_FileName = m_FilePath.substr(found+1);
    
    int numAttributes = m_PolyData->GetPointData()->GetNumberOfArrays();
    for (int j = 0; j < numAttributes; j++)
    {
        int dim = m_PolyData->GetPointData()->GetArray(j)->GetNumberOfComponents();
        const char * AttributeName = m_PolyData->GetPointData()->GetArrayName(j);
        
        if (dim == 1 || dim == 3 )
        {
            std::string AttributeString = AttributeName;
            m_AttributeList.push_back(AttributeString);
        }
        if( dim == 3)
        {
            //Vectors
            vtkPVPostFilter *  getVectors = vtkPVPostFilter::New();
            std::ostringstream strs;
            strs.str("");
            strs.clear();
            strs << AttributeName << "_mag" << std::endl;
            getVectors->DoAnyNeededConversions(m_PolyData,strs.str().c_str(),vtkDataObject::FIELD_ASSOCIATION_POINTS, AttributeName, "Magnitude");
            getVectors->Delete();
            
            //ColorMap by Direction
            //            AddColorByDirection(m_PolyData,AttributeName);
            //            AddAbsoluteColorByDirection(m_PolyData,AttributeName);
            
        }
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());
}



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
        }
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());
}



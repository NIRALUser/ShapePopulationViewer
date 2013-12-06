#include "ShapePopulationData.h"


ShapePopulationData::ShapePopulationData()
{
    m_PolyData = vtkSmartPointer<vtkPolyData>::New();
}


void ShapePopulationData::ReadMesh(std::string a_filePath)
{
    //initialize a vtkPolyDataReader to read the .vtk file
    vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New();
    meshReader->SetFileName ( a_filePath.c_str() );
    meshReader->ReadAllScalarsOn();
    meshReader->Update();

    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalGenerator->SetInput(meshReader->GetOutput());
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
        const char * AttributeName = m_PolyData->GetPointData()->GetArrayName(j);
        std::string AttributeString = AttributeName;
        m_AttributeList.push_back(AttributeString);

        if(m_PolyData->GetPointData()->GetArray(j)->GetNumberOfComponents() == 3)
        {
            vtkPVPostFilter *  test = vtkPVPostFilter::New();
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << AttributeName << "_mag" << std::endl;
            test->DoAnyNeededConversions(m_PolyData,strs.str().c_str(),vtkDataObject::FIELD_ASSOCIATION_POINTS, AttributeName, "Magnitude");
        }
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());
}

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

    int numScalars = m_PolyData->GetPointData()->GetNumberOfArrays();
	for (int j = 0; j < numScalars; j++)
	{
        const char * AttributeName = m_PolyData->GetPointData()->GetArrayName(j);

		std::string AttributeString = AttributeName;
        m_AttributeList.push_back(AttributeString);
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());

    m_Position = "Original";
}


int ShapePopulationData::SetPosition(std::string a_Position)
{
    if ( strcmp(a_Position.c_str(),"Centered") == 0 || strcmp(a_Position.c_str(),"Original") == 0)
    {
        m_Position = a_Position;
        return 0;
    }
    else return -1;
}

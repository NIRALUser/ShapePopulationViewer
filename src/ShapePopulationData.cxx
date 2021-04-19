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

vtkSmartPointer<vtkPolyData> ShapePopulationData::ReadMesh(std::string a_filePath)
{
    if (endswith(a_filePath, ".vtp") || endswith(a_filePath, ".vtk"))
    {
        vtkSmartPointer<vtkPolyData> polyData = ReadPolyData(a_filePath);
        if(polyData == NULL) return 0;

        ReadMesh(polyData, a_filePath);
    }
    else if (endswith(a_filePath, ".xml"))
    {
        vtkNew<vtkXMLDataParser> parser;
        parser->SetFileName(a_filePath.c_str());
        if( parser->Parse() != 1) return 0;

        auto* root = parser->GetRootElement();
        vtkSmartPointer<vtkPolyData> upPD = ReadPolyData(root->FindNestedElementWithName("upSpoke")->GetCharacterData());
        vtkSmartPointer<vtkPolyData> downPD = ReadPolyData(root->FindNestedElementWithName("downSpoke")->GetCharacterData());
        vtkSmartPointer<vtkPolyData> crestPD = ReadPolyData(root->FindNestedElementWithName("crestSpoke")->GetCharacterData());
        if(upPD == NULL || downPD == NULL || crestPD == NULL) return 0;

        ReadSRep(upPD, downPD, crestPD, a_filePath);
    }
    else
    {
        return 0;
    }
    return m_PolyData;
}

void ShapePopulationData::ReadMesh(vtkPolyData* polyData, const std::string& a_filePath)
{
    size_t found = a_filePath.find_last_of("/\\");
    if (found != std::string::npos)
    {
        m_FileDir = a_filePath.substr(0,found);
        m_FileName = a_filePath.substr(found+1);
    }
    else
    {
        m_FileName = a_filePath;
    }

    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
    normalGenerator->SetInputData(polyData);
    normalGenerator->SplittingOff();
    normalGenerator->ComputePointNormalsOn();
    normalGenerator->ComputeCellNormalsOff();
    normalGenerator->Update();

    //Update the class members
    m_PolyData = normalGenerator->GetOutput();

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

void ShapePopulationData::ReadSRep(vtkPolyData* upPD, vtkPolyData* downPD, vtkPolyData* crestPD, const std::string& a_filePath)
{
    size_t found = a_filePath.find_last_of("/\\");
    if (found != std::string::npos)
    {
        m_FileDir = a_filePath.substr(0,found);
        m_FileName = a_filePath.substr(found+1);
    }
    else
    {
        m_FileName = a_filePath;
    }

    // merge poly data
    vtkNew<vtkAppendPolyData> append;
    append->AddInputData(upPD);
    append->AddInputData(downPD);
    append->AddInputData(crestPD);
    append->Update();

    //Update the class members
    m_PolyData = append->GetOutput();

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



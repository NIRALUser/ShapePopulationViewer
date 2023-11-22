#include "ShapePopulationData.h"

#include <qSlicerCoreApplication.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSRepNode.h>
#include <vtkMRMLSRepStorageNode.h>
#include <vtkSlicerApplicationLogic.h>
#include <vtkSlicerSRepLogic.h>
#include <vtkSRepExportPolyDataProperties.h>

namespace {

class ScopedRemove {
public:
    ScopedRemove(vtkMRMLNode* toRemove, vtkMRMLScene* scene)
        : m_ToRemove(toRemove)
        , m_Scene(scene)
    {}

    void cancel() {
        m_ToRemove = nullptr;
        m_Scene = nullptr;
    }

    ~ScopedRemove() {
        if (m_ToRemove && m_Scene) {
            m_Scene->RemoveNode(m_ToRemove);
            m_ToRemove = nullptr;
            m_Scene = nullptr;
        }
    }
private:
    vtkMRMLNode* m_ToRemove;
    vtkMRMLScene* m_Scene;
};

}

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
    else if (endswith(a_filePath, ".srep.json"))
    {
        ReadSRep(a_filePath);
    }
    else if(endswith(a_filePath, ".fcsv"))
    {
        ReadFiducial(a_filePath);
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

    // Create spoke geometries and merge poly data
    vtkNew<vtkAppendPolyData> append;
    append->AddInputData(ExtractSpokes(upPD, 0));
    append->AddInputData(ExtractSpokes(downPD, 4));
    append->AddInputData(ExtractSpokes(crestPD, 2));
    append->AddInputData(AttachCellType(upPD, 1));
    append->AddInputData(AttachCellType(crestPD, 3));
    append->Update();

    // Update the class members
    m_PolyData = append->GetOutput();

    int numAttributes = m_PolyData->GetPointData()->GetNumberOfArrays();
    for (int j = 0; j < numAttributes; j++)
    {
        int dim = m_PolyData->GetPointData()->GetArray(j)->GetNumberOfComponents();
        const char * AttributeName = m_PolyData->GetPointData()->GetArrayName(j);

        if (dim == 1)
        {
            std::string AttributeString = AttributeName;
            m_AttributeList.push_back(AttributeString);
        }
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());
}

void ShapePopulationData::ReadFiducial(const std::string& a_filePath)
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
  
    // Read file and record fiducial positions
    QFile inputFile(a_filePath.c_str());
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream inputStream(&inputFile);
    std::vector<std::vector<double>> sphereCenters;
    while(!inputStream.atEnd())
    {
        auto line = inputStream.readLine();
        if (line.startsWith("vtkMRMLMarkupsFiducialNode"))
        {
            QStringList list  = line.split(',');
            std::vector<double> center;
            center.push_back(list[1].toDouble());
            center.push_back(list[2].toDouble());
            center.push_back(list[3].toDouble());
            sphereCenters.push_back(center);
        }
    }
    inputFile.close();

    // Check closest distance between fiducials to decide sphere radius
    auto closestDistance = VTK_DOUBLE_MAX;
    for (int i = 0; i < (int)sphereCenters.size() - 1; ++i)
    {
        auto centerI = sphereCenters[i];
        for (int j = i + 1; j < (int)sphereCenters.size(); j++)
        {
            auto centerJ = sphereCenters[j];
            double diff[3] = {centerJ[0] - centerI[0],
                              centerJ[1] - centerI[1],
                              centerJ[2] - centerI[2]};
            double distance = std::sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
            if (distance < closestDistance)
            {
                closestDistance = distance;
            }
        }
    }

    // Create geometry for visualization
    vtkNew<vtkAppendPolyData> append;
    double radius = closestDistance * 0.2;
    for (int i = 0; i < (int)sphereCenters.size(); ++i)
    {
        vtkNew<vtkSphereSource> sphere;
        sphere->SetRadius(radius);
        sphere->SetCenter(sphereCenters[i][0], sphereCenters[i][1], sphereCenters[i][2]);
        sphere->Update();
        auto spherePD = sphere->GetOutput();
        vtkNew <vtkIntArray> intArray;
        intArray->SetNumberOfComponents(1);
        intArray->SetNumberOfValues(spherePD->GetNumberOfPoints());
        intArray->SetName("Fiducial Index");
        for (int j = 0; j < spherePD->GetNumberOfPoints(); ++j)
        {
            intArray->SetValue(j, i);
        }
        spherePD->GetPointData()->SetActiveScalars("Fiducial Index");
        spherePD->GetPointData()->SetScalars(intArray);
        append->AddInputData(spherePD);
    }
    append->Update();

    m_PolyData = append->GetOutput();

    int numAttributes = m_PolyData->GetPointData()->GetNumberOfArrays();
    for (int j = 0; j < numAttributes; j++)
    {
        int dim = m_PolyData->GetPointData()->GetArray(j)->GetNumberOfComponents();
        const char * AttributeName = m_PolyData->GetPointData()->GetArrayName(j);

        if (dim == 1)
        {
            std::string AttributeString = AttributeName;
            m_AttributeList.push_back(AttributeString);
        }
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());
}

vtkSmartPointer<vtkPolyData> ShapePopulationData::ExtractSpokes(vtkPolyData* polyData, int cellType)
{
    int nPoints = polyData->GetNumberOfPoints();
    auto* pointData = polyData->GetPointData();
    auto* arr_length = pointData->GetArray("spokeLength");
    auto* arr_dirs = pointData->GetArray("spokeDirection");

    vtkNew<vtkPoints> spokePoints;
    vtkNew<vtkCellArray> spokeLines;
    vtkNew<vtkIntArray> typeArray;
    typeArray->SetName("cellType");
    typeArray->SetNumberOfComponents(1);

    for (int i  = 0; i < nPoints; ++i)
    {
        auto* pt0 = polyData->GetPoint(i);
        auto spoke_length = arr_length->GetTuple1(i);
        auto* dir = arr_dirs->GetTuple3(i);
        double pt1[] = {pt0[0] + spoke_length * dir[0],
                        pt0[1] + spoke_length * dir[1],
                        pt0[2] + spoke_length * dir[2]};
        int id0 = spokePoints->InsertNextPoint(pt0);
        int id1 = spokePoints->InsertNextPoint(pt1);

        vtkNew<vtkLine> arrow;
        arrow->GetPointIds()->SetId(0, id0);
        arrow->GetPointIds()->SetId(1, id1);
        spokeLines->InsertNextCell(arrow);
        typeArray->InsertNextValue(cellType);
        typeArray->InsertNextValue(cellType);
    }

    vtkNew<vtkPolyData> spokePD;
    spokePD->SetPoints(spokePoints);
    spokePD->SetLines(spokeLines);
    spokePD->GetPointData()->SetActiveScalars("cellType");
    spokePD->GetPointData()->SetScalars(typeArray);
    return spokePD;
}

vtkSmartPointer<vtkPolyData> ShapePopulationData::AttachCellType(vtkPolyData *polyData, int cellType)
{
    vtkNew<vtkIntArray> outputType;
    outputType->SetName("cellType");
    outputType->SetNumberOfComponents(1);
    for (int i = 0; i < polyData->GetNumberOfPoints(); ++i)
    {
        outputType->InsertNextValue(cellType);
    }

    polyData->GetPointData()->SetActiveScalars("cellType");
    polyData->GetPointData()->SetScalars(outputType);
    return polyData;
}

void ShapePopulationData::ReadSRep(const std::string& a_filePath)
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

    vtkMRMLScene* mrmlScene = qSlicerCoreApplication::application()->applicationLogic()->GetMRMLScene();
    if (!mrmlScene) {
        std::cerr << "ShapePopulationData::ReadSRep Unable to get application MRML scene!" << std::endl;
        return;
    }
    vtkMRMLSRepStorageNode* storageNode = vtkMRMLSRepStorageNode::SafeDownCast(mrmlScene->AddNewNodeByClass("vtkMRMLSRepStorageNode"));
    if (!storageNode) {
        std::cerr << "ShapePopulationData::ReadSRep Unable to get storageNode!" << std::endl;
        return;
    }
    ScopedRemove removeStorage(storageNode, mrmlScene);
    storageNode->SetFileName(a_filePath.c_str());

    vtkMRMLSRepNode* srepNode = storageNode->CreateSRepNode("srep");
    if (!srepNode) {
        std::cerr << "ShapePopulationData::ReadSRep Unable to get srepNode!" << std::endl;
        return;
    }
    ScopedRemove removeSRep(srepNode, mrmlScene);
    /// removing the srep node will remove its associated storage node
    removeStorage.cancel();

    auto spokeExportProps = vtkSmartPointer<vtkSRepExportPolyDataProperties>::New();
    spokeExportProps->SetIncludeSkeletalSheet(false);
    spokeExportProps->SetIncludeCrestCurve(false);
    spokeExportProps->SetIncludeSkeletonToCrestConnection(false);
    spokeExportProps->SetIncludeSpine(false);
    spokeExportProps->SetPointTypeArrayName("cellType");
    spokeExportProps->SetLineTypeArrayName("cellType");
    vtkNew<vtkIntArray> spokeTypeArray;
    spokeTypeArray->SetNumberOfTuples(vtkSRepExportPolyDataProperties::NumberOfTypes);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::UpBoundaryPointType, 0);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::UpSkeletalPointType, 0);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::DownBoundaryPointType, 4);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::DownSkeletalPointType, 4);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::CrestBoundaryPointType, 2);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::CrestSkeletalPointType, 2);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::UpSpokeLineType, 0);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::DownSpokeLineType, 4);
    spokeTypeArray->SetValue(vtkSRepExportPolyDataProperties::CrestSpokeLineType, 2);
    spokeExportProps->SetSRepDataArray(spokeTypeArray);

    auto sheetExportProps = vtkSmartPointer<vtkSRepExportPolyDataProperties>::New();
    sheetExportProps->SetIncludeUpSpokes(false);
    sheetExportProps->SetIncludeDownSpokes(false);
    sheetExportProps->SetIncludeCrestSpokes(false);
    sheetExportProps->SetIncludeSpine(false);
    sheetExportProps->SetPointTypeArrayName("cellType");
    sheetExportProps->SetLineTypeArrayName("cellType");
    vtkNew<vtkIntArray> typeArray;
    typeArray->SetNumberOfTuples(vtkSRepExportPolyDataProperties::NumberOfTypes);
    typeArray->SetValue(vtkSRepExportPolyDataProperties::UpSkeletalPointType, 1);
    typeArray->SetValue(vtkSRepExportPolyDataProperties::DownSkeletalPointType, 1);
    typeArray->SetValue(vtkSRepExportPolyDataProperties::CrestSkeletalPointType, 3);
    typeArray->SetValue(vtkSRepExportPolyDataProperties::CrestCurveLineType, 3);
    typeArray->SetValue(vtkSRepExportPolyDataProperties::SkeletalSheetLineType, 1);
    sheetExportProps->SetSRepDataArray(typeArray);

    // making two different polydatas with overlapping points of different colors
    auto logic = vtkSmartPointer<vtkSlicerSRepLogic>::New();
    vtkNew<vtkAppendPolyData> append;
    append->AddInputData(logic->SmartExportSRepToPolyData(*srepNode->GetSRep(), *spokeExportProps));
    append->AddInputData(logic->SmartExportSRepToPolyData(*srepNode->GetSRep(), *sheetExportProps));
    append->Update();
    m_PolyData = append->GetOutput();
    
    m_PolyData->GetPointData()->SetActiveScalars("cellType");
    int numAttributes = m_PolyData->GetPointData()->GetNumberOfArrays();
    for (int j = 0; j < numAttributes; j++)
    {
        int dim = m_PolyData->GetPointData()->GetArray(j)->GetNumberOfComponents();
        const char * AttributeName = m_PolyData->GetPointData()->GetArrayName(j);

        if (dim == 1)
        {
            std::string AttributeString = AttributeName;
            m_AttributeList.push_back(AttributeString);
        }
    }
    std::sort(m_AttributeList.begin(),m_AttributeList.end());
}

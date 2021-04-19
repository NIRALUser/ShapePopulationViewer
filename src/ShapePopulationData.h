#ifndef ShapePopulationData_H
#define ShapePopulationData_H

#include <vtkVersion.h>

#include <vtkAppendPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataReader.h>
#include <vtkXMLDataParser.h>
#include <vtkXMLPolyDataReader.h>

#include "vtkPVPostFilter.h"

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

class ShapePopulationData
{
    public :

    ShapePopulationData();
    ~ShapePopulationData(){}

    vtkSmartPointer<vtkPolyData> ReadMesh(std::string a_filePath);
    void ReadMesh(vtkPolyData* polyData, const std::string& a_filePath);
    void ReadSRep(vtkPolyData* upPD, vtkPolyData* downPD, vtkPolyData* crestPD, const std::string& a_filePath);
    vtkSmartPointer<vtkPolyData> ReadPolyData(std::string a_filePath);

    vtkSmartPointer<vtkPolyData> GetPolyData() {return m_PolyData;}
    std::string GetFileName() {return m_FileName;}
    std::string GetFileDir() {return m_FileDir;}
    std::vector<std::string> GetAttributeList() {return m_AttributeList;}

    protected :

    vtkSmartPointer<vtkPolyData> m_PolyData;
    std::string m_FileName;
    std::string m_FileDir;
    std::vector<std::string> m_AttributeList;
};


#endif


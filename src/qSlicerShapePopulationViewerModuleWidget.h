/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef qSlicerShapePopulationViewerModuleWidget_h
#define qSlicerShapePopulationViewerModuleWidget_h

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"

// MRML includes
class vtkMRMLModelNode;

// VTK includes
class vtkPolyData;

#include "qSlicerShapePopulationViewerModuleExport.h"

class qSlicerShapePopulationViewerModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_SHAPEPOPULATIONVIEWER_EXPORT qSlicerShapePopulationViewerModuleWidget :
  public qSlicerAbstractModuleWidget
{
    Q_OBJECT

public:

    typedef qSlicerAbstractModuleWidget Superclass;
    qSlicerShapePopulationViewerModuleWidget(QWidget *parent=0);
    virtual ~qSlicerShapePopulationViewerModuleWidget();

    virtual void enter();
    virtual void exit();

public slots:
    void loadSelectedModel();
    void loadModel(vtkMRMLModelNode* modelNode);
    void loadModel(const QString& filePath);
    void loadModel(vtkPolyData* polydata, const QString& modelName);
    void loadCSVFile(const QString& filePath);
    void loadSRep(const QString& filePath);
    void loadFiducial(const QString& filePath);

    void deleteModels();

    void setVectorScale(double value);
    void setVectorDensity(double value);
    void displayVectors(bool display);

protected slots:
    void onMRMLSceneNodeAddedEvent(vtkObject*,vtkObject*);
    void onMRMLNodeModified(vtkObject*);

protected:
    QScopedPointer<qSlicerShapePopulationViewerModuleWidgetPrivate> d_ptr;

    virtual void setup();

private:
    Q_DECLARE_PRIVATE(qSlicerShapePopulationViewerModuleWidget)
    Q_DISABLE_COPY(qSlicerShapePopulationViewerModuleWidget)
};

#endif

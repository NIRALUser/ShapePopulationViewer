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

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

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

protected:
    QScopedPointer<qSlicerShapePopulationViewerModuleWidgetPrivate> d_ptr;

    virtual void setup();

private:
    Q_DECLARE_PRIVATE(qSlicerShapePopulationViewerModuleWidget)
    Q_DISABLE_COPY(qSlicerShapePopulationViewerModuleWidget)
};

#endif

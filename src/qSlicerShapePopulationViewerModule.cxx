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

// ShapePopulationViewer includes
#include "qSlicerShapePopulationViewerModule.h"
#include "qSlicerShapePopulationViewerModuleWidget.h"

//-----------------------------------------------------------------------------
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtPlugin>
Q_EXPORT_PLUGIN2(qSlicerShapePopulationViewerModule, qSlicerShapePopulationViewerModule);
#endif

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerShapePopulationViewerModulePrivate
{
public:
    qSlicerShapePopulationViewerModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerShapePopulationViewerModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerModulePrivate::qSlicerShapePopulationViewerModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerShapePopulationViewerModule methods

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerModule::qSlicerShapePopulationViewerModule(QObject* _parent)
    : Superclass(_parent)
    , d_ptr(new qSlicerShapePopulationViewerModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerModule::~qSlicerShapePopulationViewerModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerShapePopulationViewerModule::helpText() const
{
    return "Visualize and interact with multiple surfaces at the same time to easily compare them.";
}

//-----------------------------------------------------------------------------
QString qSlicerShapePopulationViewerModule::acknowledgementText() const
{
    return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerShapePopulationViewerModule::contributors() const
{
    QStringList moduleContributors;
    moduleContributors << QString("Alexis Girault (NIRAL, UNC)");
    moduleContributors << QString("Francois Budin (NIRAL, UNC)");
    moduleContributors << QString("Laura Pascal (DCBIA-OrthoLab, UofM)");
    return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerShapePopulationViewerModule::icon() const
{
    return QIcon(":/resources/logo.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerShapePopulationViewerModule::categories() const
{
    return QStringList() << "Shape Analysis";
}

//-----------------------------------------------------------------------------
QStringList qSlicerShapePopulationViewerModule::dependencies() const
{
    return QStringList() << "SRep";
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModule::setup()
{
    this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerShapePopulationViewerModule
::createWidgetRepresentation()
{
    return new qSlicerShapePopulationViewerModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerShapePopulationViewerModule::createLogic()
{
    return 0;
}

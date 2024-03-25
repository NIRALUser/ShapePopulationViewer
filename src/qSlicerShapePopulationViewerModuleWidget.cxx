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

// Qt includes
#include <QAction>
#include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QMainWindow>
#include <QToolButton>

// MRML includes
#include <vtkMRMLLayoutLogic.h>
#include <vtkMRMLLayoutNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLScene.h>

// Slicer includes
#include <qSlicerApplication.h>
#include <qSlicerLayoutManager.h>

// ShapePopulationViewer includes
#include "qSlicerShapePopulationViewerModuleWidget.h"
#include "qSlicerShapePopulationViewerLayoutViewFactory.h"
#include "ui_qSlicerShapePopulationViewerModuleWidget.h"
#include "ShapePopulationQT.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerShapePopulationViewerModuleWidgetPrivate: public Ui_qSlicerShapePopulationViewerModuleWidget
{
public:
    qSlicerShapePopulationViewerModuleWidgetPrivate();

    ShapePopulationQT* ShapePopulationWidget;
    int SavedViewArrangement;
    bool WasDataProbeVisible;
    bool WasCentralWidgetAutoFillBackground;
};

//-----------------------------------------------------------------------------
// qSlicerShapePopulationViewerModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerModuleWidgetPrivate::qSlicerShapePopulationViewerModuleWidgetPrivate()
{
    this->ShapePopulationWidget = 0;
    this->SavedViewArrangement = vtkMRMLLayoutNode::SlicerLayoutInitialView;
    this->WasDataProbeVisible = true;
    this->WasCentralWidgetAutoFillBackground = false;
}

//-----------------------------------------------------------------------------
// qSlicerShapePopulationViewerModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerModuleWidget::qSlicerShapePopulationViewerModuleWidget(QWidget* _parent)
    : Superclass( _parent )
    , d_ptr( new qSlicerShapePopulationViewerModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerModuleWidget::~qSlicerShapePopulationViewerModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::enter()
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);

    this->Superclass::enter();

    qSlicerApplication* app = qSlicerApplication::application();

    // Save AutoFillBackground
    QFrame* centralWidgetLayoutFrame = app->mainWindow()->findChild<QFrame*>("CentralWidgetLayoutFrame");
    QWidget* centralWidget = qobject_cast<QWidget*>(centralWidgetLayoutFrame->parent());
    d->WasCentralWidgetAutoFillBackground = centralWidget->autoFillBackground();

    // Save view arrangement
    vtkMRMLLayoutNode* layoutNode = app->layoutManager()->layoutLogic()->GetLayoutNode();
    d->SavedViewArrangement = layoutNode->GetViewArrangement();

    // Save DataProbe visbility
    QWidget* dataProbeCollapsibleWidget = app->mainWindow()->findChild<QWidget*>("DataProbeCollapsibleWidget");
    d->WasDataProbeVisible = dataProbeCollapsibleWidget->isVisible();

    // Disable AutoFillBackground
    centralWidget->setAutoFillBackground(false);

    // Select ShapePopulationViewer custom layout
    layoutNode->SetViewArrangement(vtkMRMLLayoutNode::SlicerLayoutUserView);

    // Hide DataProbe
    dataProbeCollapsibleWidget->setVisible(false);

    // Force redraw
    d->ShapePopulationWidget->setVisible(false);
    d->ShapePopulationWidget->setVisible(true);

    this->qvtkConnect(this->mrmlScene(), vtkMRMLScene::NodeAddedEvent,
                      this, SLOT(onMRMLSceneNodeAddedEvent(vtkObject*, vtkObject*)));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::exit()
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);

    this->Superclass::exit();

    qSlicerApplication* app = qSlicerApplication::application();

    // Restore AutoFillBackground
    QFrame* centralWidgetLayoutFrame = app->mainWindow()->findChild<QFrame*>("CentralWidgetLayoutFrame");
    QWidget* centralWidget = qobject_cast<QWidget*>(centralWidgetLayoutFrame->parent());
    centralWidget->setAutoFillBackground(true);

    // Restore view arrangement
    vtkMRMLLayoutNode* layoutNode = app->layoutManager()->layoutLogic()->GetLayoutNode();
    layoutNode->SetViewArrangement(d->SavedViewArrangement);

    // Restore DataProbe visibility
    QWidget* dataProbeCollapsibleWidget = app->mainWindow()->findChild<QWidget*>("DataProbeCollapsibleWidget");
    dataProbeCollapsibleWidget->setVisible(d->WasDataProbeVisible);

    this->qvtkDisconnectAll();
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::setup()
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->setupUi(this);
    this->Superclass::setup();

    Q_INIT_RESOURCE(ShapePopulationViewer);

    qSlicerApplication* app = qSlicerApplication::application();

    d->ShapePopulationWidget = new ShapePopulationQT;
    d->ShapePopulationWidget->setPalette(app->palette());

    app->layoutManager()->registerViewFactory(
                new qSlicerShapePopulationViewerLayoutViewFactory(d->ShapePopulationWidget, app->layoutManager()));

    vtkMRMLLayoutNode* layoutNode = app->layoutManager()->layoutLogic()->GetLayoutNode();
    Q_ASSERT(layoutNode);

    const char* shapePopulationViewerLayout =
    "<layout type=\"horizontal\">"
    " <item>"
    "  <shapepopulationviewer></shapepopulationviewer>"
    " </item>"
    "</layout>";
    layoutNode->AddLayoutDescription(
                vtkMRMLLayoutNode::SlicerLayoutUserView, shapePopulationViewerLayout);

    // Delete Selection & All
    d->toolButton_Delete_Selection->setDefaultAction(d->ShapePopulationWidget->actionDelete);
    d->toolButton_Delete_All->setDefaultAction(d->ShapePopulationWidget->actionDelete_All);

    // Import
    d->toolButton_Open_Directory->setDefaultAction(d->ShapePopulationWidget->actionOpen_Directory);
    d->toolButton_Open_VTK_Files->setDefaultAction(d->ShapePopulationWidget->actionOpen_VTK_Files);
    d->toolButton_Load_CSV->setDefaultAction(d->ShapePopulationWidget->actionLoad_CSV);
    d->toolButton_Load_Time_Series->setDefaultAction(d->ShapePopulationWidget->actionLoad_Time_Series);
    d->toolButton_Open_SRep_Files->setDefaultAction(d->ShapePopulationWidget->actionOpen_SRep_Files);
    d->toolButton_Open_Fiducial_Files->setDefaultAction(d->ShapePopulationWidget->actionOpen_Fiducial_Files);

    // Export
#ifdef ShapePopulationViewer_HAS_EXPORT_SUPPORT
    bool exportVisible = true;
    foreach(QAction* action, QList<QAction*>()
            << d->ShapePopulationWidget->actionTo_PDF
            << d->ShapePopulationWidget->actionTo_SVG
            << d->ShapePopulationWidget->actionTo_TEX
            << d->ShapePopulationWidget->actionTo_PS
            << d->ShapePopulationWidget->actionTo_EPS
            )
    {
        QToolButton* button = new QToolButton();
        button->setDefaultAction(action);
        d->ExportLayout->addWidget(button);
    }
#else
    bool exportVisible = false;
#endif
    d->ExportCTKCollapsibleButton->setVisible(exportVisible);

    // Settings
    foreach(QAction* action, QList<QAction*>()
            << d->ShapePopulationWidget->actionCameraConfig
            << d->ShapePopulationWidget->actionBackgroundConfig
            << d->ShapePopulationWidget->actionLoad_Colorbar
            << d->ShapePopulationWidget->actionSave_Colorbar)
    {
        QToolButton* button = new QToolButton();
        button->setDefaultAction(action);
        d->SettingsLayout->addWidget(button);
    }

    connect(d->ModelLoadPushButton, SIGNAL(clicked()), this, SLOT(loadSelectedModel()));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadSelectedModel()
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    this->loadModel(vtkMRMLModelNode::SafeDownCast(d->ModelNodeComboBox->currentNode()));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadModel(vtkMRMLModelNode* modelNode)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->loadModel(modelNode);
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadModel(const QString& filePath)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->loadVTKFilesCLP(QFileInfoList() << QFileInfo(filePath));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadModel(vtkPolyData* polyData, const QString& modelName)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->loadModel(polyData, modelName);
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadCSVFile(const QString& filePath)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    QFileInfoList modelFileInfoList;
    foreach(const QString& modelFilePath, ShapePopulationQT::parseCSVFile(filePath))
    {
        modelFileInfoList << QFileInfo(modelFilePath);
    }
    d->ShapePopulationWidget->loadVTKFilesCLP(modelFileInfoList);
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadSRep(const QString &filePath)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->loadSRepFilesCLP(QFileInfoList() << QFileInfo(filePath));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::loadFiducial(const QString &filePath)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->loadFiducialFilesCLP(QFileInfoList() << QFileInfo(filePath));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::deleteModels()
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->actionDelete_All->trigger();
}

void qSlicerShapePopulationViewerModuleWidget::setVectorScale(double value){
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->slider_vectorScale->setValue((int) (value * 100));
}

void qSlicerShapePopulationViewerModuleWidget::setVectorDensity(double value){
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->slider_arrowDens->setValue((int) (value * 100));
}

void qSlicerShapePopulationViewerModuleWidget::displayVectors(bool display){
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    d->ShapePopulationWidget->checkBox_displayVectors->setChecked(display);
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::onMRMLSceneNodeAddedEvent(vtkObject *vtkNotUsed(caller), vtkObject *callData)
{
    Q_D(qSlicerShapePopulationViewerModuleWidget);
    vtkMRMLModelNode * modelNode = vtkMRMLModelNode::SafeDownCast(callData);
    if (modelNode == NULL)
        {
        return;
        }
    if (!d->ModelAutomaticLoadingCheckBox->isChecked())
    {
        return;
    }
    qvtkConnect(modelNode, vtkCommand::ModifiedEvent, this, SLOT(onMRMLNodeModified(vtkObject*)));
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerModuleWidget::onMRMLNodeModified(vtkObject *caller)
{

    vtkMRMLModelNode * modelNode = vtkMRMLModelNode::SafeDownCast(caller);
    if (modelNode == NULL)
    {
        return;
    }
    if (modelNode->GetStorageNode() == NULL)
    {
        return;
    }
    this->loadModel(modelNode);
    qvtkDisconnect(modelNode, vtkCommand::ModifiedEvent, this, SLOT(onMRMLNodeModified(vtkObject*)));
}

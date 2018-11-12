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
#include <QList>
#include <QMainWindow>
#include <QToolButton>

// MRML includes
#include <vtkMRMLLayoutLogic.h>
#include <vtkMRMLLayoutNode.h>

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

    // Actions
    foreach(QAction* action, QList<QAction*>()
            << d->ShapePopulationWidget->actionOpen_Directory
            << d->ShapePopulationWidget->actionOpen_VTK_Files
            << d->ShapePopulationWidget->actionLoad_CSV
            << d->ShapePopulationWidget->actionDelete
            << d->ShapePopulationWidget->actionDelete_All)
    {
        QToolButton* button = new QToolButton();
        button->setDefaultAction(action);
        d->ActionsLayout->addWidget(button);
    }

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
}

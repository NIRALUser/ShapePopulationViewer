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
#include "qSlicerShapePopulationViewerLayoutViewFactory.h"
#include "ShapePopulationQT.h"

//-----------------------------------------------------------------------------
class qSlicerShapePopulationViewerLayoutViewFactoryPrivate
{
  Q_DECLARE_PUBLIC(qSlicerShapePopulationViewerLayoutViewFactory);
public:
  qSlicerShapePopulationViewerLayoutViewFactoryPrivate(qSlicerShapePopulationViewerLayoutViewFactory& object);
  virtual ~qSlicerShapePopulationViewerLayoutViewFactoryPrivate();

  virtual void init();

  ShapePopulationQT* ShapePopulationWidget;

protected:
  qSlicerShapePopulationViewerLayoutViewFactory* q_ptr;
};

//-----------------------------------------------------------------------------
// qSlicerShapePopulationViewerLayoutViewFactoryPrivate methods

qSlicerShapePopulationViewerLayoutViewFactoryPrivate
::qSlicerShapePopulationViewerLayoutViewFactoryPrivate(qSlicerShapePopulationViewerLayoutViewFactory& object)
  : ShapePopulationWidget(0), q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerLayoutViewFactoryPrivate::~qSlicerShapePopulationViewerLayoutViewFactoryPrivate()
{
}

//-----------------------------------------------------------------------------
void qSlicerShapePopulationViewerLayoutViewFactoryPrivate::init()
{
}

//-----------------------------------------------------------------------------
// qSlicerShapePopulationViewerLayoutViewFactory methods

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerLayoutViewFactory::qSlicerShapePopulationViewerLayoutViewFactory(ShapePopulationQT* shapePopulationWidget, QObject* parent)
  : Superclass(parent)
  , d_ptr(new qSlicerShapePopulationViewerLayoutViewFactoryPrivate(*this))
{
  Q_D(qSlicerShapePopulationViewerLayoutViewFactory);
  d->init();
  this->setUseCachedViews(false);
  d->ShapePopulationWidget = shapePopulationWidget;
}

//-----------------------------------------------------------------------------
qSlicerShapePopulationViewerLayoutViewFactory::~qSlicerShapePopulationViewerLayoutViewFactory()
{
}

//-----------------------------------------------------------------------------
QStringList qSlicerShapePopulationViewerLayoutViewFactory::supportedElementNames()const
{
  return QStringList() << "shapepopulationviewer";
}

//---------------------------------------------------------------------------
QWidget* qSlicerShapePopulationViewerLayoutViewFactory::createViewFromXML(QDomElement layoutElement)
{
    Q_D(qSlicerShapePopulationViewerLayoutViewFactory);
    if (layoutElement.tagName() == "shapepopulationviewer")
        {
        return d->ShapePopulationWidget;
        }
    return 0;
}

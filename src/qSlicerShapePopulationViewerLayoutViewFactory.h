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

#ifndef qSlicerShapePopulationViewerLayoutViewFactory_h
#define qSlicerShapePopulationViewerLayoutViewFactory_h

// CTK includes
#include "ctkLayoutViewFactory.h"

// ShapePopulationViewer includes
#include "qSlicerShapePopulationViewerModuleExport.h"
class ShapePopulationQT;
class qSlicerShapePopulationViewerLayoutViewFactoryPrivate;

class Q_SLICER_QTMODULES_SHAPEPOPULATIONVIEWER_EXPORT qSlicerShapePopulationViewerLayoutViewFactory
 : public ctkLayoutViewFactory
{
  Q_OBJECT
public:
  typedef ctkLayoutViewFactory Superclass;
  qSlicerShapePopulationViewerLayoutViewFactory(ShapePopulationQT* shapePopulationWidget, QObject* parent);
  virtual ~qSlicerShapePopulationViewerLayoutViewFactory();

  /// Reimplemented to support 'shapepopulationviewer'
  virtual QStringList supportedElementNames()const;

protected:
  QScopedPointer<qSlicerShapePopulationViewerLayoutViewFactoryPrivate> d_ptr;

  /// Reimplemented to instantiate the welcome screen widget.
  virtual QWidget* createViewFromXML(QDomElement layoutElement);

private:
  Q_DECLARE_PRIVATE(qSlicerShapePopulationViewerLayoutViewFactory)
};

#endif

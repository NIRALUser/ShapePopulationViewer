#include <QApplication>
#include "ShapePopulationViewer.h"

int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );
  
  ShapePopulationViewer ShapePopulationViewer;
  ShapePopulationViewer.show();
  
  return app.exec();
}

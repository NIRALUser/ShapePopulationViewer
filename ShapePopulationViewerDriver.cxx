#include <QApplication>
#include "ShapePopulationViewer.h"

/**
 * Standard driver for a Qt gui.  Create the Gui object, show it and call exec to enter the gui's main event loop.
 * @brief main
 * @param argc
 * @param argv
 * @return value returned by gui's exit function.
 */
int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );
  
  ShapePopulationViewer ShapePopulationViewer;
  ShapePopulationViewer.show();
  
  return app.exec();
}

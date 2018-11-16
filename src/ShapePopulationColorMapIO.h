#ifndef ShapePopulationColorMapIO_h
#define ShapePopulationColorMapIO_h

// ShapePopulationViewer includes
#include "ShapePopulationStruct.h"

// Qt includes
#include <QString>
class QWidget;

// STD includes
#include <vector>

namespace ShapePopulationColorMapIO
{
void loadColorPointList(QString a_filePath, std::vector<colorPointStruct> * a_colorPointList, QWidget* parent=0);

void saveColorPointList(QString a_filePath, const std::vector<colorPointStruct>& colorPointList, QWidget* parent=0);
}


#endif

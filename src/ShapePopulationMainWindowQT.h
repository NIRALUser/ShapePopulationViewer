#ifndef SHAPEPOPULATIONMAINWINDOWQT_H
#define SHAPEPOPULATIONMAINWINDOWQT_H

#include "ui_ShapePopulationMainWindowQT.h"

class ShapePopulationQT;

class ShapePopulationMainWindowQT : public QMainWindow, public Ui::ShapePopulationMainWindowQT
{
    Q_OBJECT

public:
    ShapePopulationMainWindowQT();
    ~ShapePopulationMainWindowQT();

    ShapePopulationQT* shapePopulation();
};

#endif

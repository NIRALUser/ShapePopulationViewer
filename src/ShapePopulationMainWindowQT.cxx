
#include "ShapePopulationMainWindowQT.h"
#include "ShapePopulationQT.h"

ShapePopulationMainWindowQT::ShapePopulationMainWindowQT()
{
    this->setupUi(this);

    this->menuFile->addAction(this->shapePopulation()->actionOpen_Directory);
    this->menuFile->addAction(this->shapePopulation()->actionOpen_VTK_Files);
    this->menuFile->addAction(this->shapePopulation()->actionLoad_CSV);
    this->menuFile->addAction(this->shapePopulation()->actionOpen_SRep_Files);
    this->menuFile->addSeparator();
    this->menuFile->addMenu(this->menuExport);
    this->menuFile->addSeparator();
    this->menuFile->addAction(this->shapePopulation()->actionDelete);
    this->menuFile->addAction(this->shapePopulation()->actionDelete_All);
    this->menuFile->addSeparator();
    this->menuFile->addAction(this->actionExit);

    this->menuExport->addAction(this->shapePopulation()->actionTo_PDF);
    this->menuExport->addAction(this->shapePopulation()->actionTo_PS);
    this->menuExport->addAction(this->shapePopulation()->actionTo_EPS);
    this->menuExport->addAction(this->shapePopulation()->actionTo_TEX);
    this->menuExport->addAction(this->shapePopulation()->actionTo_SVG);

    this->menuOptions->addAction(this->shapePopulation()->actionCameraConfig);
    this->menuOptions->addAction(this->shapePopulation()->actionBackgroundConfig);
    this->menuOptions->addSeparator();
    this->menuOptions->addAction(this->shapePopulation()->actionLoad_Colorbar);
    this->menuOptions->addAction(this->shapePopulation()->actionSave_Colorbar);

    // Menu signals
    connect(this->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

#if defined(Q_OS_UNIX)
    Ui_ShapePopulationMainWindowQT::menuBar->setNativeMenuBar(false);
#endif
}

ShapePopulationMainWindowQT::~ShapePopulationMainWindowQT()
{
}

ShapePopulationQT* ShapePopulationMainWindowQT::shapePopulation()
{
    return qobject_cast<ShapePopulationQT*>(this->centralWidget());
}

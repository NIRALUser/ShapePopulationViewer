#include <QApplication>
#include <QFileInfo>

#include "ShapePopulationQT.h"
#include "ShapePopulationViewerCLP.h"

#ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLWIDGET
# ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLNATIVEWIDGET
#  include <QVTKOpenGLNativeWidget.h>
# else
#  include <QVTKOpenGLWidget.h>
# endif
#endif

#include <iostream>
#include <stdio.h>

void fileDoesNotExist(std::string file, ShapePopulationQT *windowPointer)
{
    std::ostringstream strs;
    strs << file << std::endl
         << "Does not exist."<< std::endl;
    QMessageBox::critical(windowPointer,"File/Directory not found",QString(strs.str().c_str()), QMessageBox::Ok);
}

void wrongFileFormat(std::string file, std::string format, ShapePopulationQT *windowPointer)
{
    std::ostringstream strs;
    strs << file << std::endl
         << "This file is not a "<<format<<" File"<< std::endl;
    QMessageBox::critical(windowPointer,"Wrong file format",QString(strs.str().c_str()), QMessageBox::Ok);
}

void checkConfigurationFiles(std::string cameraConfig, std::string colormapConfig, ShapePopulationQT *windowPointer)
{
    if(!cameraConfig.empty())
    {
        QString cameraConfigPath(cameraConfig.c_str());
        QFileInfo cameraConfigInfo(cameraConfigPath);
        if (!cameraConfigPath.endsWith(".pvcc")) wrongFileFormat(cameraConfig,"pvcc", windowPointer);   // Control the files format
        else if(!cameraConfigInfo.exists()) fileDoesNotExist(cameraConfig, windowPointer);              // Control that the file exists
        else windowPointer->loadCameraCLP(cameraConfig);
    }
    if(!colormapConfig.empty())
    {
        QString colormapConfigPath(colormapConfig.c_str());
        QFileInfo colormapConfigInfo(colormapConfigPath);
        if (!colormapConfigPath.endsWith(".spvcm")) wrongFileFormat(colormapConfig,"spvcm", windowPointer);   // Control the files format
        else if(!colormapConfigInfo.exists()) fileDoesNotExist(colormapConfig, windowPointer);                // Control that the file exists
        else windowPointer->loadColorMapCLP(colormapConfig);
    }
}


//---------------------------------------//
int main( int argc, char** argv )
{
    PARSE_ARGS;

    // Set default surface format.
#ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLWIDGET
# ifdef ShapePopulationViewer_VTK_USE_QVTKOPENGLNATIVEWIDGET
    QSurfaceFormat format = QVTKOpenGLNativeWidget::defaultFormat();
# else
    QSurfaceFormat format = QVTKOpenGLWidget::defaultFormat();
# endif
    QSurfaceFormat::setDefaultFormat(format);
#endif

    // QT SOFTWARE
    QApplication app( argc, argv );
    Q_INIT_RESOURCE(ShapePopulationViewer);
    ShapePopulationQT window;
    window.show();
    window.raise();

    // CLP
    if(!vtkFiles.empty())
    {
        bool load = false;
        QFileInfoList fileList;
        for(unsigned int i = 0 ; i < vtkFiles.size(); i++)
        {
            QString QFilePath(vtkFiles[i].c_str());
            QFileInfo vtkFileInfo(QFilePath);
            if (!QFilePath.endsWith(".vtk") && !QFilePath.endsWith(".vtp")) wrongFileFormat(vtkFiles[i],"vtk/vtp", &window);         // Control the files format
            else if(!vtkFileInfo.exists()) fileDoesNotExist(vtkFiles[i], &window);                  // Control that the file exists
            else
            {
                fileList.append(vtkFileInfo);
                load = true;
            }
        }
        if(load == true)
        {
            window.loadVTKFilesCLP(fileList);
            checkConfigurationFiles(cameraConfig, colormapConfig, &window);
        }
    }
    if(!vtkDirectory.empty())
    {
        QDir vtkDir(vtkDirectory.c_str());
        if(!vtkDir.exists()) fileDoesNotExist(vtkDirectory, &window);
        else
        {
            window.loadVTKDirCLP(vtkDir);
            checkConfigurationFiles(cameraConfig, colormapConfig, &window);
        }
    }
    if(!CSVFile.empty())
    {
        QString QFilePath(CSVFile.c_str());
        QFileInfo CSVFileInfo(QFilePath);
        if (!QFilePath.endsWith(".csv")) wrongFileFormat(CSVFile,"csv", &window);         // Control the files format
        else if(!CSVFileInfo.exists()) fileDoesNotExist(CSVFile, &window);                  // Control that the file exists
        else
        {
            window.loadCSVFileCLP(CSVFileInfo);
            checkConfigurationFiles(cameraConfig, colormapConfig, &window);
        }
    }
    return app.exec();

    return 0;
}


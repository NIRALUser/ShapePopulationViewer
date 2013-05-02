#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>
#include "QVTKWidget.h"
#include <QVector>
#include <QApplication>
#include "ShapePopulationViewer.h"
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>
#include "QVTKWidget.h"
#include <QVector>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include "vtkSmartPointer.h"
#include <vtkCamera.h>
#include <vtkPolyDataReader.h>
#include <QFileDialog>
#include <QVectorIterator>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfoListIterator>
#include <QFileInfo>
#include <iostream>
#include <stdio.h>


// Test that given a dir selected using qt, our code will find the correct absolute path of all the VTK files in that dir
//@author Joe Waggoner
int main() 
{	
	
	QString dir ="/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars";
	QDir vtkDir(dir);
	QFileInfoList list = vtkDir.entryInfoList();
    	vtkSmartPointer<vtkRenderer> headRenderer = vtkSmartPointer<vtkRenderer>::New();
    	int root = 0;
	int total = 0;
	int expectedTotal = 6;
	QStringList expPathsVector;
	expPathsVector <<"/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars/N0012-2-01_segmented_vent_pp_surfSPHARM_procalign.vtk" <<"/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars/N0012-2-02_segmented_vent_pp_surfSPHARM_procalign.vtk" << "/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars/N0012-2-03_segmented_vent_pp_surfSPHARM_procalign.vtk" << "/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars/N0052-2-01_segmented_vent_pp_surfSPHARM_procalign.vtk" << "/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars/N0052-2-02_segmented_vent_pp_surfSPHARM_procalign.vtk" <<  "/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars/N0052-2-03_segmented_vent_pp_surfSPHARM_procalign.vtk";
    	for ( ;root*root < list.size()-2;root++);
    		int col = 0;
    		int row = 0;
    	for (int j = 0; j < list.size(); j++) {
        	QString path = list.at(j).absoluteFilePath();
		if(!path.endsWith(".vtk")) continue;
			total = total+1;
			cout << "num: " << j;
  		for(int k = 0; k<expectedTotal; k++){
			if(QString::compare(path, expPathsVector.at(k))==0){
				break;
			}
			if(k == expectedTotal-1){
				return 1;			
			}
			
    		}
    
    	if(total ==expectedTotal){
		return 0;
	}	
	}
	return 1;

}



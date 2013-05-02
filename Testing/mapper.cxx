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

//If we get the correct number of non empty meshes initialized, then we will get passed test, otherwise fail.
//@author Joe Waggoner
int main(){


	QString dir ="/home/joe/Dropbox/ShapePopulationViewerData/exampleData/withoutScalars";
    	QDir vtkDir(dir);
    	QFileInfoList list = vtkDir.entryInfoList();
    	vtkSmartPointer<vtkRenderer> headRenderer = vtkSmartPointer<vtkRenderer>::New();
    	int root = 0;
    	for ( ;root*root < list.size()-2;root++);
    	int col = 0;
    	int row = 0;
	double* bounds = new double[6];
    	for (int j = 0; j < list.size(); j++) {
        	QString path = list.at(j).absoluteFilePath();
		if(!path.endsWith(".vtk")) continue;
		QByteArray arr = path.toLatin1();
		const char *filename = arr.data();
        	vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New() ;
		meshReader->SetFileName ( filename ) ;
		meshReader->Update () ;
		vtkSmartPointer<vtkPolyDataMapper> mapper =vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(meshReader->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		bounds = mapper->GetBounds();
		for(int k = 0; k<6; k++){
			if((bounds[k]!=-1)&&(bounds[k]!=1)){
				break;			
			}
			if (k==5){
				return 1;
			}
		}
		
    	}








return 0;

}

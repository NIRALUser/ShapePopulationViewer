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
#include <cstdlib>

/*tests for the qvector module
we will test value, size, append, clear, because these are the only methods we use*/

int main() 
{	

	QVector<int> *integerVector = new QVector<int> (0);
	int failed = 0;

	if(integerVector->size() != 0)
		failed+=1;

	for (int i = 0; i <10; i++){
		integerVector->append(i);
	}
	for (int i = 0; i<10; i++){
		if(integerVector->value(i) != i)
			failed++;	
	}

	if(integerVector->size() != 10)
		failed+=1;

	integerVector->clear();
	if(integerVector->size() != 0)
		failed+=1;
	for (int i = 0; i <10; i++){
		integerVector->append(10-i);
	}
	for (int i = 0; i<10; i++){
		if(integerVector->value(i) != 10-i)
			failed++;	
	}
	
	if (failed !=0)//failed a test
		return 1;

    //randomized iterated tests
    integerVector->clear();
    int trueSize = 0;
    for (int i = 0; i < 500000; i++) {
        int ran = rand() % 3;
        if (ran == 0) {
            integerVector->append(trueSize);
            trueSize++;
            if (integerVector->size() != trueSize || integerVector->value(trueSize-1) != trueSize - 1)
                return 1;
        } else if (ran == 1) {
            integerVector->clear();
            if (integerVector->size() != 0)
                return 1;
            trueSize = 0;
        } else if (ran == 2) {
            if (integerVector->size() == 0)
                continue;
            int randInd = rand() % integerVector->size();
            if (integerVector->value(randInd) != randInd)
                return 1;
        }
        
    }
    return 0;


}




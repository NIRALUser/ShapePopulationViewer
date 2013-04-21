#include "ShareCameraQt.h"
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
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkLookupTable.h"
#include "vtkPolyData.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeMapper.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkProperty.h"
#include <QDebug>
#include "vtkFloatArray.h"
#include "vtkScalarBarActor.h"
#include <QCheckBox>
#include <QMessageBox>
#include "vtkPolyDataNormals.h"
#include <QSize>
#include <QResizeEvent>
#include <QInputDialog>
#include <QStringList>
#include <QRegExp>
#include "vtkPolyDataWriter.h"
#include <QEvent>
#include "vtkScalarsToColors.h"
#include <QIcon>

// Constructor
ShareCameraQt::ShareCameraQt() 
{
    this->setupUi(this);
    synced = true;
    //this->dockWidget->scrollAreaContents = this->scrollAreaWidgetContents;
    this->widgetList = new QVector<QVTKWidget *>(20);
    this->polyList = new QVector<vtkPolyData *>(20);
    this->mapperList = new QVector<vtkPolyDataMapper *>(20);
    this->colorMaps = new QVector<QString>(20);
    this->phi = 1;
    this->PI = 3.14159265358979323;
    this->loaded = 0;
    //this->checkBox->setCheckState(Qt::Checked);
    // Set up action signals and slots
    QString path = QDir::currentPath();
    QIcon down(path + "/arrows/down.jpg");
    QIcon up(path + "/arrows/up.jpg");
    QIcon left(path + "/arrows/left.jpg");
    QIcon right(path + "/arrows/right.jpg");
    QIcon ur(path + "/arrows/upper_right.jpg");
    QIcon ll(path + "/arrows/lower_left.jpg");
    //this->toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toolButton->setIcon(ur);
    //this->toolButton_2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toolButton_2->setIcon(ll);
    //this->toolButton_3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toolButton_3->setIcon(right);
    //this->toolButton_4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toolButton_4->setIcon(left);
    //this->toolButton_5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toolButton_5->setIcon(up);
    //this->toolButton_6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->toolButton_6->setIcon(down);
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->actionFlip_Meshes,SIGNAL(triggered()),this,SLOT(flipMeshes()));
    connect(this->actionWrite_Back_Meshes,SIGNAL(triggered()),this,SLOT(writeMeshes()));
    connect(this->actionOpen_vtk_Files,SIGNAL(triggered()),this,SLOT(openVTKS()));
}

void ShareCameraQt::ModifiedHandler() 
{

    if (!synced)
        return;
    for (int i = 0; i < this->widgetList->size();i++) {
        this->widgetList->value(i)->GetRenderWindow()->Render();
    }
}

void ShareCameraQt::slotExit() 
{
  qApp->exit();
}

//void ShareCameraQt::on_pushButton_clicked()
//{
//    loaded = 1;
//    for (int i = 0; i < this->widgetList->size(); i++) {
//        QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
//        layout->removeWidget(this->widgetList->value(i));
//        delete this->widgetList->value(i);
//    }
//    this->widgetList->clear();
//    QString dir = QFileDialog::getExistingDirectory(this,tr("Open .vtk Directory"),"~",QFileDialog::ShowDirsOnly);
//    QDir vtkDir(dir);
//    this->directory = vtkDir;
//    //this->lineEdit_3->selectAll();
//    //this->lineEdit_3->insert(vtkDir.absolutePath());
//    this->updateWidgets();
//}
void ShareCameraQt::updateWidgets() {
    for (int i = 0; i < this->widgetList->size(); i++) {
        QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
        layout->removeWidget(this->widgetList->value(i));
       // this->widgetList->value(i)->reparent(NULL,Qt::Widget,);
        delete this->widgetList->value(i);
    }
    this->widgetList->clear();
    this->polyList->clear();
    this->mapperList->clear();
    this->comboBox->clear();
    QFileInfoList list = this->directory.entryInfoList();
    int meshes = 0;
    vtkSmartPointer<vtkRenderer> headRenderer = vtkSmartPointer<vtkRenderer>::New();
    for (int j = 0; j < list.size(); j++) {
        QString path = list.at(j).absoluteFilePath();
        if (!path.endsWith(".vtk"))
            continue;
        QByteArray arr = path.toLatin1();
        const char *filename = arr.data();
        vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New() ;
        meshReader->SetFileName ( filename );
        meshReader->ReadAllScalarsOn();
        //meshReader->SetScalarsName("scalarsPhi_original");

        const char *scalars;
        meshReader->Update();
        vtkPolyData *polydata = meshReader->GetOutput();
        int numScalars = polydata->GetPointData()->GetNumberOfArrays();
        for (int i = 0; i < numScalars;i++) {
            QString scalarName(polydata->GetPointData()->GetArrayName(i));
            if (meshes == 0) {
                this->comboBox->addItem(scalarName);
                this->colorMaps->append(scalarName);
            }
            if (this->phi && scalarName.endsWith("Phi_original")) {
                QByteArray bytes = scalarName.toLatin1();
                scalars = bytes.data();
                polydata->GetPointData()->SetActiveScalars(scalars);
            } else if (!this->phi && scalarName.endsWith("Theta_original")) {
                QByteArray bytes = scalarName.toLatin1();
                scalars = bytes.data();
                polydata->GetPointData()->SetActiveScalars(scalars);
                            //polydata->GetPointData()->SetActiveScalars(scalars);
            }
        }

      //  polydata->GetPointData()->SetActiveScalars(scalars);
//        if (this->phi)
//            polydata->GetPointData()->SetActiveScalars("Color_Map_Phi_original");
//        else
//            polydata->GetPointData()->SetActiveScalars("Color_Map_Theta_original");
        polydata->Update();
        vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
        #if VTK_MAJOR_VERSION <= 5
           normalGenerator->SetInput(polydata); // polydata is your mesh, the one you put in the mapper
        #else
           normalGenerator->SetInputData(polydata);
        #endif
        normalGenerator->ComputePointNormalsOn();
        normalGenerator->ComputeCellNormalsOff();
        normalGenerator->Update();
        // Optional settings
        normalGenerator->SetFeatureAngle(30.0); // put paraview parameters here
        normalGenerator->SetSplitting(0);
        normalGenerator->SetConsistency(0);
        normalGenerator->SetAutoOrientNormals(0);
        normalGenerator->SetComputePointNormals(1);
        normalGenerator->SetComputeCellNormals(0);
        normalGenerator->SetFlipNormals(0);
        normalGenerator->SetNonManifoldTraversal(1);
        polydata = normalGenerator->GetOutput();
        this->polyList->append(polydata);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        #if VTK_MAJOR_VERSION <= 5
            mapper->SetInputConnection(polydata->GetProducerPort());
        #else
            mapper->SetInputData(polydata);
        #endif

        vtkColorTransferFunction* DistanceMapTFunc = vtkColorTransferFunction::New();
        DistanceMapTFunc->SetColorSpaceToRGB();

        double rangeLUT[2];
        rangeLUT[0] = 0;
        rangeLUT[1] = (this->phi) ? 6.2832: 3.1459;
        DistanceMapTFunc->AdjustRange(rangeLUT);
        DistanceMapTFunc->SetColorSpaceToDiverging();
        DistanceMapTFunc->RemoveAllPoints(); //
        DistanceMapTFunc->AddRGBPoint(rangeLUT[0], 0, 255, 0); // we add a point in the LUT to enforce the min value to be green = 0,255,0
        DistanceMapTFunc->AddRGBPoint( (fabs(rangeLUT[1] - rangeLUT[0]) ) / 2, 255, 255, 0); // we add another point in the middle of the range to be yellow = 255,255,0
        DistanceMapTFunc->AddRGBPoint(rangeLUT[1], 255, 0, 0);
        DistanceMapTFunc->ClampingOn();

        mapper->SetLookupTable( DistanceMapTFunc );
        mapper->ScalarVisibilityOn();
        if (this->phi)
            mapper->SetScalarRange(0,6.2832);
        else
            mapper->SetScalarRange(0,3.1459);
        mapper->SetScalarModeToUsePointData();
        mapper->SetColorModeToMapScalars();
        mapper->Update();
        meshes++;
        char buffer[30];
        sprintf(buffer,"Mesh %d",meshes);
        bool check = polydata->GetPointData()->GetScalars() != NULL;
        vtkSmartPointer<vtkScalarBarActor> scalarBar =
            vtkSmartPointer<vtkScalarBarActor>::New();
        if (check) {
            scalarBar->SetLookupTable(mapper->GetLookupTable());
            scalarBar->SetTitle(buffer);

            scalarBar->SetNumberOfLabels(4);
        }
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        //actor->GetProperty()->SetOpacity(.995);
        actor->SetMapper(mapper);
        this->mapperList->append(mapper);
        if (j == 0) {
            headRenderer->AddActor(actor);
            if (check)
                headRenderer->AddActor2D(scalarBar);
            QVTKWidget *next = new QVTKWidget(this->scrollAreaWidgetContents);
            this->widgetList->append(next);
            next->GetRenderWindow()->AddRenderer(headRenderer);
            next->GetRenderWindow()->SetAlphaBitPlanes(1);
            headRenderer->ResetCamera();
        } else {
            vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
            renderer->AddActor(actor);
            if (check)
                renderer->AddActor2D(scalarBar);
            QVTKWidget *next = new QVTKWidget(this->scrollAreaWidgetContents);
            this->widgetList->append(next);
            next->GetRenderWindow()->AddRenderer(renderer);
            next->GetRenderWindow()->SetAlphaBitPlanes(1);
            renderer->SetActiveCamera(headRenderer->GetActiveCamera());
            renderer->ResetCamera();
        }

    }
    int root = 0;
    for ( ;root*root < this->widgetList->size();root++);
    bool square = root*root == this->widgetList->size();
    this->prevCols = root;
    int row = this->widgetList->size()/root;
    if (this->widgetList->size()%root != 0)
        row++;
    this->prevRows = row;
    this->lineEdit->selectAll();
    char buffer[30];
    int col = 0;
    row = 0;
    for (int i = 0; i < this->widgetList->size();i++) {
        QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
        layout->addWidget(this->widgetList->value(i),row,col);
        if (col == root-1) {
            col = 0;
            row++;
        } else {
            col++;
        }
    }
    sprintf(buffer,"%d",root);
    QString line(buffer);
    this->lineEdit->insert(line);
    for (int i = 0; i < this->widgetList->size();i++) {
        this->widgetList->value(i)->GetRenderWindow()->Render();
    }

    for (int i = 0; i < this->widgetList->size(); i++)
        this->widgetList->value(i)->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, this, &ShareCameraQt::ModifiedHandler);
}
void ShareCameraQt::updateCMaps() {
    for (int i = 0; i < this->polyList->size(); i++) {
        QByteArray arr = this->cmap.toLatin1();
        const char *cmap  = arr.data();

        this->polyList->at(i)->GetPointData()->SetActiveScalars(cmap);
        this->polyList->at(i)->Update();

        vtkColorTransferFunction* DistanceMapTFunc = vtkColorTransferFunction::SafeDownCast(this->mapperList->at(i)->GetLookupTable());
        DistanceMapTFunc->SetColorSpaceToRGB();

        double *rangeLUT;
        rangeLUT = this->polyList->at(i)->GetPointData()->GetScalars()->GetRange();
//        rangeLUT[0] = 0;
//        rangeLUT[1] = (this->phi) ? 2*PI: PI;
        DistanceMapTFunc->AdjustRange(rangeLUT);
        DistanceMapTFunc->SetColorSpaceToDiverging();
        DistanceMapTFunc->RemoveAllPoints(); //
        DistanceMapTFunc->AddRGBPoint(rangeLUT[0], 0, 255, 0); // we add a point in the LUT to enforce the min value to be green = 0,255,0
        DistanceMapTFunc->AddRGBPoint( (fabs(rangeLUT[1] - rangeLUT[0]) ) / 2, 255, 255, 0); // we add another point in the middle of the range to be yellow = 255,255,0
        DistanceMapTFunc->AddRGBPoint(rangeLUT[1], 255, 0, 0);
        DistanceMapTFunc->ClampingOn();

        this->mapperList->at(i)->SetLookupTable( DistanceMapTFunc );
        this->mapperList->at(i)->ScalarVisibilityOn();
        if (this->phi)
            this->mapperList->at(i)->SetScalarRange(0,6.2832);
        else
            this->mapperList->at(i)->SetScalarRange(0,3.1459);
        this->mapperList->at(i)->SetScalarModeToUsePointData();
        this->mapperList->at(i)->SetColorModeToMapScalars();
        this->mapperList->at(i)->Update();
    }
}

//void ShareCameraQt::on_checkBox_toggled(bool checked)
//{
//    if (checked)
//        this->checkBox_2->setCheckState(Qt::Unchecked);
//    this->phi = 1;
//    if (loaded) {
//        this->updateCMaps();
//        this->ModifiedHandler();
//    }
//}
void ShareCameraQt::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    QSize dockSize = this->dockWidget->size();
    //this->dockWidgetContents->resize(dockSize.width(),dockSize.height() - 30);
    this->scrollArea->resize(dockSize.width()-20,dockSize.height() - 50);
    QSize size = this->scrollArea->size();
    this->scrollAreaWidgetContents->resize(dockSize.width()-20,dockSize.height());
}

//void ShareCameraQt::on_checkBox_2_toggled(bool checked)
//{
//    if (checked)
//        this->checkBox->setCheckState(Qt::Unchecked);
//    this->phi = 0;
//    if (loaded) {
//        this->updateCMaps();
//        this->ModifiedHandler();
//    }
//}
void ShareCameraQt::flipMeshes() {
    QString response = QInputDialog::getText(this,"Flip some meshes","Enter the mesh numbers you want to flip in CSV format");
    QStringList tokens = response.split(",",QString::SkipEmptyParts);
    for (int i = 0; i < tokens.size(); i++) {
        QString toke = tokens.at(i);
        int index = toke.toInt();
        if (index < 0 || index > this->polyList->size() - 1)
            continue;
        vtkFloatArray *scalars = vtkFloatArray::SafeDownCast(this->polyList->at(index-1)->GetPointData()->GetScalars());
        if (scalars == NULL)
            continue;
        char *scalarName = this->polyList->at(index - 1)->GetPointData()->GetScalars()->GetName();
        vtkFloatArray *newScalars = vtkFloatArray::New();
        newScalars->SetName(scalarName);
        for (int j = 0; j < scalars->GetNumberOfTuples();j++) {
            float scalar = scalars->GetValue(j);
            scalar = (this->phi) ? (scalar > PI) ? 3*PI - scalar: PI - scalar: (scalar > PI/2) ? 3*PI/2 - scalar: PI/2 - scalar;
            newScalars->InsertNextValue(scalar);
        }
        this->polyList->at(index-1)->GetPointData()->SetScalars(newScalars);
        this->polyList->at(index-1)->Update();

    }
    this->ModifiedHandler();
}
void ShareCameraQt::writeMeshes() {
    QFileInfoList list = this->directory.entryInfoList();
    int meshes = 0;
    for (int i = 0; i < list.size(); i++) {
        QString path = list.at(i).absoluteFilePath();
        if (!path.endsWith(".vtk"))
            continue;
        QByteArray arr = path.toLatin1();
        const char *filename = arr.data();
        vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
        writer->SetFileName(filename);
        writer->SetInput(this->polyList->at(meshes++));
        writer->Update();

    }
}
void ShareCameraQt::openVTKS() {
    loaded = 1;
    for (int i = 0; i < this->widgetList->size(); i++) {
        QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
        layout->removeWidget(this->widgetList->value(i));
        delete this->widgetList->value(i);
    }
    this->widgetList->clear();
    QString dir = QFileDialog::getExistingDirectory(this,tr("Open .vtk Directory"),"~",QFileDialog::ShowDirsOnly);
    QDir vtkDir(dir);
    this->directory = vtkDir;
    this->updateWidgets();
}

void ShareCameraQt::on_checkBox_9_toggled(bool checked)
{
    if (checked) {
        this->checkBox_10->setCheckState(Qt::Unchecked);
        this->scrollAreaSize = this->scrollAreaWidgetContents->size();
        this->scrollArea->setWidgetResizable(true);
    }
    else {
        this->scrollArea->setWidgetResizable(false);
        this->scrollAreaWidgetContents->resize(this->scrollAreaSize.width(),this->scrollAreaSize.height());
    }
}

void ShareCameraQt::on_checkBox_10_toggled(bool checked)
{
    if (checked) {
        this->checkBox_9->setCheckState(Qt::Unchecked);
        QString cols = this->lineEdit->text();
        int colNum = cols.toInt();
        if (cols == 0 || this->widgetList->size() == 0 || this->prevCols == colNum)
            return;
        int rows = this->widgetList->size()/colNum;
        if (this->widgetList->size()%colNum != 0)
            rows++;
        QSize size = this->scrollAreaWidgetContents->size();
        int width = size.width();//12 can be replaced with whatever the margins for scrollAreaWidgetContents' layout are
        int prevWidth = (width - 12*(2 + this->prevCols - 1))/this->prevCols;
        int prevHeight = (size.height() - 12*(2 + this->prevRows - 1))/this->prevRows;
        int moreCols = colNum - this->prevCols;
        int moreRows = rows - this->prevRows;
        this->scrollAreaWidgetContents->resize(size.width() + 12 + moreCols*prevWidth,size.height() + moreRows*prevHeight + 12);
        this->prevCols = colNum;
        this->prevRows = rows;
        int col = 0;
        int row = 0;
        for (int i = 0; i < this->widgetList->size();i++) {
            QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
            layout->addWidget(this->widgetList->value(i),row,col);
            if (col == colNum-1) {
                col = 0;
                row++;
            } else {
                col++;
            }
        }
    } else {
        //do nothing I guess...
    }
}

void ShareCameraQt::on_lineEdit_editingFinished()
{
    if (this->checkBox_10->checkState() == Qt::Checked) {
        this->on_checkBox_10_toggled(true);
    }
}


void ShareCameraQt::on_checkBox_3_toggled(bool checked)
{
    synced = !checked;
}

void ShareCameraQt::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QString text = this->comboBox->currentText();
    this->cmap = text;
    this->updateCMaps();
    this->ModifiedHandler();
}

void ShareCameraQt::on_toolButton_clicked()
{
    viewChange(0,0,1);

}


void ShareCameraQt::viewChange(int x, int y, int z){
    if(loaded==0){
        return;
    }
    vtkRendererCollection* collection = this->widgetList->value(0)->GetRenderWindow()->GetRenderers();
    vtkRenderer* firstRenderer = collection->GetFirstRenderer();
    this->coords  = firstRenderer->GetActiveCamera()->GetFocalPoint();
    this->distance = firstRenderer->GetActiveCamera()->GetDistance();
    double* positions =  firstRenderer->GetActiveCamera()->GetPosition();
    double roll = firstRenderer->GetActiveCamera()->GetRoll();
    firstRenderer->GetActiveCamera()->SetPosition(coords[0]+x*distance,coords[1]+y*distance,coords[2]+z*distance);
    //setroll to .001, because it breaks on y axis if roll = 0
    firstRenderer->GetActiveCamera()->SetRoll(.001);
    bool temp = this->synced;
    this->synced = true;
    this->ModifiedHandler();
    this->synced = temp;
}


void ShareCameraQt::on_toolButton_2_clicked()
{
    viewChange(0,0,-1);
}

void ShareCameraQt::on_toolButton_3_clicked()
{
    viewChange(1,0,0);
}

void ShareCameraQt::on_toolButton_4_clicked()
{
    viewChange(-1,0,0);
}

void ShareCameraQt::on_toolButton_5_clicked()
{
    viewChange(0,1,0);
}

void ShareCameraQt::on_toolButton_6_clicked()
{
    viewChange(0,-1,0);
}

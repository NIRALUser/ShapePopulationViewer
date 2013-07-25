#include "ShapePopulationViewer.h"

// Basics
#include <vtkSmartPointer.h>
#include <sstream>
#include <string.h>

// PolyData
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>

// Scalars
//#include vvtkDataArray.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
//#include <vtkScalarsToColors.h>

// Visualization
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <QVTKWidget.h>

// Window Annotations
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>

// Transfer Function
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>

// QT
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QCheckBox>
#include <QSize>
#include <QResizeEvent>
#include <QInputDialog>
#include <QRegExp>
#include <QEvent>
#include <QIcon>
#include <QMouseEvent>


#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleTrackballActor.h>
// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Constructor for ShapePopulationViewer GUI, it will initialize model vectors, connect some callbacks and also draw the arrow icons.
 * @brief ShapePopulationViewer::ShapePopulationViewer
 * @author Michael Guarino & Alexis Girault
 */
ShapePopulationViewer::ShapePopulationViewer()
{
    this->setupUi(this);

    //Vector initialization
    this->headcam = vtkCamera::New();
    this->windowList = new QVector<vtkRenderWindow *>(20);           //to get the widgets, renderwindow, renderer, and camera
    this->widgetList = new QVector<QVTKWidget *>(20);           //to get the widgets, renderwindow, renderer, and camera
    this->mapperList = new QVector<vtkPolyDataMapper *>(20);    //to modify mappers when the colormap is updated
    this->loaded = 0;

    // Set up Axes buttons
    QString path = QDir::currentPath();
    QIcon down(path + "/arrows/down.jpg");
    QIcon up(path + "/arrows/up.jpg");
    QIcon left(path + "/arrows/left.jpg");
    QIcon right(path + "/arrows/right.jpg");
    QIcon ur(path + "/arrows/upper_right.jpg");
    QIcon ll(path + "/arrows/lower_left.jpg");
    this->toolButton_1->setIcon(ur);
    this->toolButton_2->setIcon(ll);
    this->toolButton_3->setIcon(right);
    this->toolButton_4->setIcon(left);
    this->toolButton_5->setIcon(up);
    this->toolButton_6->setIcon(down);

    // Set up action signals and slots
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->actionFlip_Meshes,SIGNAL(triggered()),this,SLOT(flipMeshes()));
    connect(this->actionWrite_Back_Meshes,SIGNAL(triggered()),this,SLOT(writeMeshes()));
    connect(this->actionOpen_vtk_Files,SIGNAL(triggered()),this,SLOT(openVTKS()));
}


/**
 * Close function for the gui.
 * @brief ShapePopulationViewer::slotExit
 * @author Michael Guarino
 */
void ShapePopulationViewer::slotExit()
{
  qApp->exit();
}


// DISPLAY FUNCTIONS ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Helper function which reads a directory filled with .vtk files and renders each polydata within in a
 * separate QVTKWidget.  All model vectors are filled as well.
 * @brief ShapePopulationViewer::updateWidgets
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::updateWidgets()
{
    //clear any Content from the layout
    QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
    for (int i = 0; i < this->widgetList->size(); i++)
    {
        layout->removeWidget(this->widgetList->value(i));
        delete this->widgetList->value(i);
    }

    //clear all vectors so they might be refilled
    this->windowList->clear();
    this->widgetList->clear();
    this->mapperList->clear();
    this->colorMapBox->clear();

    //initializations
    QFileInfoList list = this->directory.entryInfoList(); //pull the filenames from the directory
    int meshesNumber = 0; //number of .vtk files to be

    //upload and visualization of all the .vtk files
    for (int i = 0; i < list.size(); i++)
    {
        //get filepath of current file, convert it to ascii chars if .vtk file
        QString QFilePath = list.at(i).canonicalFilePath();
        QString QFileName = list.at(i).fileName();
        if (!QFilePath.endsWith(".vtk")) //if not a .vtk extension, analyze next file in the 'for'
        {
            continue;
        }
        else
        {
            meshesNumber++;
        }
        QByteArray path = QFilePath.toLatin1();
        const char *filePath = path.data();
        QByteArray nam = QFileName.toLatin1();
        const char *fileName = nam.data();

        //initialize a vtkPolyDataReader to read the .vtk file
        vtkSmartPointer<vtkPolyDataReader> meshReader = vtkSmartPointer<vtkPolyDataReader>::New() ;
        meshReader->SetFileName ( filePath );
        meshReader->ReadAllScalarsOn();//make sure we are reading scalars
        meshReader->Update();//wire read setting preparation
        vtkPolyData *polydata = meshReader->GetOutput();//read the file : polydata is our mesh



        //smooth the image using a normal generator
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
        normalGenerator->SetFeatureAngle(30.0);     // put paraview parameters here
        normalGenerator->SetSplitting(0);           // DO NOT SPLIT, this will result in the mesh duplicating points, which could affect comparison
        normalGenerator->SetConsistency(0);         //
        normalGenerator->SetAutoOrientNormals(0);   //
        normalGenerator->SetComputePointNormals(1); // Other initialization specific, see vtkPolyDataNormals
        normalGenerator->SetComputeCellNormals(0);  //
        normalGenerator->SetFlipNormals(0);         //
        normalGenerator->SetNonManifoldTraversal(1);//
        polydata = normalGenerator->GetOutput();


        // Get Datas in points
        int numScalars = polydata->GetPointData()->GetNumberOfArrays();
        for (int j = 0; j < numScalars; j++)
        {
            QString scalarName(polydata->GetPointData()->GetArrayName(j));
            if (meshesNumber == 1)//only do this for the first mesh (all others should be the same, otherwise there would be no point in a batched comparison
            {
                this->colorMapBox->addItem(scalarName);
            }
            QByteArray bytes = scalarName.toLatin1();
            const char * scalars = bytes.data();
            polydata->GetPointData()->SetActiveScalars(scalars);
        }

        /*
         * Begin vtk initialization pipeline, generally speaking, you pass a polydata through a mapper, then an actor, then a renderer
         */

        //MAPPER
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        this->mapperList->append(mapper);//add the mapper to our mapperlist for easy retrieval
        #if VTK_MAJOR_VERSION <= 5
            mapper->SetInputConnection(polydata->GetProducerPort());
        #else
            mapper->SetInputData(polydata);
        #endif

        // Annotate the image with his name (and his number)
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation->SetLinearFontScaleFactor( 2 );
        cornerAnnotation->SetNonlinearFontScaleFactor( 1 );
        cornerAnnotation->SetMaximumFontSize( 15 );
        cornerAnnotation->SetText( 2,fileName);
/*      std::stringstream strs;
        strs << (i-1);
        std::string temp_str = strs.str();
        const char* fileNumber = temp_str.c_str();
        cornerAnnotation->SetText( 3,fileNumber); //problem if files not a vtk file!
*/

        //ACTOR
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        //RENDERER
        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddViewProp(cornerAnnotation);
        renderer->AddActor(actor);

        //DATAS
        if (polydata->GetPointData()->GetScalars() != NULL)
        {

            vtkSmartPointer<vtkColorTransferFunction> DistanceMapTFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
            double *rangeLUT = polydata->GetPointData()->GetScalars()->GetRange();
            updateCMaps(mapper, DistanceMapTFunc, rangeLUT);

            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar->SetLookupTable(mapper->GetLookupTable());
            scalarBar->SetNumberOfLabels(3);
            scalarBar->SetMaximumWidthInPixels(60);

            vtkSmartPointer<vtkTextProperty> LabelProperty = vtkSmartPointer<vtkTextProperty>::New();
            LabelProperty->SetFontSize(12);
            LabelProperty->ShadowOn();
            scalarBar->SetLabelTextProperty(LabelProperty);

            renderer->AddActor2D(scalarBar);
        }

        //CAMERA
        renderer->SetActiveCamera(headcam);//set the active camera for this renderer to main camera
        renderer->ResetCamera();

        //QVTKWIDGET
        QVTKWidget *meshWidget = new QVTKWidget(this->scrollAreaWidgetContents);
        this->widgetList->append(meshWidget);
        meshWidget->GetRenderWindow()->AddRenderer(renderer);

        //SELECTION
        meshWidget->GetInteractor()->AddObserver(vtkCommand::StartInteractionEvent, this, &ShapePopulationViewer::SelectedWidget);

        /*
         *End vtk initialization pipeline
         */
    }

    if (meshesNumber == 0) return;//we did not encounter a mesh : quit

    // Enable buttons and give the maximum to the slider
    loaded = 1;
    actionFlip_Meshes->setDisabled(false);
    actionWrite_Back_Meshes->setDisabled(true); // todo
    axisButton->setDisabled(false);
    radioButton_1->setDisabled(false);
    radioButton_2->setDisabled(false);
    checkBox_synchro->setDisabled(false);
    radioButton_4->setDisabled(false);
    radioButton_5->setDisabled(false);
    radioButton_6->setDisabled(true); // todo : move or select
    radioButton_7->setDisabled(true); // todo
    colNumberTXT->setDisabled(false);
    colNumberEdit->setDisabled(false);
    colNumberSlider->setDisabled(false);
    colNumberSlider->setMaximum(meshesNumber);
    colorMapBox->setDisabled(false);

    //Identify the best number of columns for first display
    int colNumber = 0;
    int sum = 0;
    int nextOdd = 1;
    for ( ;sum < this->widgetList->size();colNumber++,nextOdd += 2)
        sum += nextOdd;//simple integer square root, will give the ceiling of the colNumber => cols >= rows

    //Display the number of columns in colNumberEdit
    printColNumber(colNumber+1);

    //Display Widgets
    on_colNumberEdit_editingFinished();

    //Start with a delayed synchro
    on_radioButton_2_toggled();

    //Start with all meshes selected
    on_checkBox_synchro_toggled(true);

    //Display Colormap (also before in the for??)
    on_colorMapBox_currentIndexChanged();
}




/**
 * Permits to get the ID of the widget selected.
 * @brief ShapePopulationViewer::SelectedWidget
 * @author Alexis Girault
 */
void ShapePopulationViewer::SelectedWidget(vtkObject* selectedObject, unsigned long ulong, void* )
{
    if(checkBox_synchro->isChecked()) return; // Dont' do anything if the synchro is on "All"

    //Get the interactor used
    QVTKInteractor* selectedInteractor = (QVTKInteractor*)selectedObject;
    vtkRenderWindow * selectedWindow = selectedInteractor->GetRenderWindow();

    //if the renderwindow already is in the renderwindowlist
    if(this->windowList->contains(selectedWindow)) return;

    // If new selection (Ctrl not pushed)
    if(selectedInteractor->GetControlKey()==0)
    {
        for (int i = 0; i < this->windowList->size();i++) //reset all backgrounds and cameras
        {
            this->windowList->value(i)->GetRenderers()->GetFirstRenderer()->SetBackground(0,0,0);
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();

            camera->DeepCopy(headcam);
            this->windowList->value(i)->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
            this->windowList->value(i)->Render();
        }
        this->windowList->clear(); // empty the selected windows list
    }

    //Background color to grey
    selectedWindow->GetRenderers()->GetFirstRenderer()->SetBackground(0.2,0.0,0.4);

    //Set to headcam
    headcam->DeepCopy(selectedWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera());
    selectedWindow->GetRenderers()->GetFirstRenderer()->SetActiveCamera(headcam);

    //Add to the windowList
    this->windowList->append(selectedWindow);
}


/**
 * Handler to any modified event sent by a QVTKWidget in the viewport.
 * The handler calls render on all the windows provided user is viewing in synchronized mode.
 * @brief ShapePopulationViewer::ModifiedHandler
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::ModifiedHandler()
{
    for (int i = 0; i < this->windowList->size();i++) //render all windows selected (one of them will be the event window)
    {
        this->windowList->value(i)->Render();
    }
}


/**
 * Helper function which sets the working color map to the one saved in the cmap QString instance variable.  Generally speaking, this
 * function will pull the specified colormap name, create a new color transfer function and add it to its mapper.
 * @brief ShapePopulationViewer::updateCMaps
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::updateCMaps(vtkPolyDataMapper*  mapper, vtkColorTransferFunction* DistanceMapTFunc, double *rangeLUT)
{
    DistanceMapTFunc->AdjustRange(rangeLUT);
    DistanceMapTFunc->SetColorSpaceToDiverging();                                           //this is necessary for the color transfer function to automatically interpolate between the points we set
    DistanceMapTFunc->RemoveAllPoints();
    DistanceMapTFunc->AddRGBPoint(rangeLUT[0], 0, 255, 0);                                  // we add a point in the LUT to enforce the min value to be green = 0,255,0
    DistanceMapTFunc->AddRGBPoint( (fabs(rangeLUT[1] - rangeLUT[0]) ) /2, 255, 255, 0);     // we add another point in the middle of the range to be yellow = 255,255,0
    DistanceMapTFunc->AddRGBPoint(rangeLUT[1], 255, 0, 0);                                  // we add a last point in the LUT to enforce the max value to be red = 255,0,0
    DistanceMapTFunc->ClampingOn();//out of range values go to either max or min

    mapper->SetLookupTable( DistanceMapTFunc );
    //mapper->SetScalarRange(rangeLUT[0],rangeLUT[1]);
    mapper->ScalarVisibilityOn();
    //mapper->SetScalarModeToUsePointData();  //we want to use point scalars (could have been cell)
    //mapper->SetColorModeToMapScalars();
    mapper->Update();
}


/**
 * Overrides QMainWindow's resize event to also help resize the contents of the dockwidget's scrollarea.
 * There is no other easy way to do this with the Qt tools as they are currently.
 * @brief ShapePopulationViewer::resizeEvent
 * @param event - variable containing the event data for a resize (needed for a super call)
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::resizeEvent(QResizeEvent *event)
{
    //Resizing Windows
    QMainWindow::resizeEvent(event);
    QSize dockSize = this->dockWidget->size();

    //According to the View Options
    if (this->radioButton_5->isChecked() == true )//view square meshes
    {
        resizeWidgetInArea();
    }

}


// PLACING FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 * @brief ShapePopulationViewer::printColNumber
 * @param colNumber
 * @author Alexis Girault
 */
void ShapePopulationViewer::printColNumber(int colNumber)
{
    char buffer[30];
    sprintf(buffer,"%d",colNumber);
    QString line(buffer);
    this->colNumberEdit->selectAll();
    this->colNumberEdit->insert(line);
}

/**
 * Function which returns the number of columns to display, useful when resizing or rearranging the widgets in a certain amount of columns.
 * @brief ShapePopulationViewer::getNumberOfColumns
 * @author Alexis Girault
 */
int ShapePopulationViewer::getNumberOfColumns()
{
    int meshesNumber = this->widgetList->size();

    QString QStr_colNumber = this->colNumberEdit->text();
    int colNumber = QStr_colNumber.toInt();

    if(colNumber > meshesNumber)
    {
        colNumber = meshesNumber;
        printColNumber(colNumber);
    }
    else if(colNumber <= 0)
    {
        colNumber = 1;
        printColNumber(colNumber);
    }
    return colNumber;
}

/**
 * Function which returns the number of rows to display, depending on the number of columns asked by the user.
 * It is used to display the widgets in a square frame.
 * @brief ShapePopulationViewer::getNumberOfRows
 * @param colNumber
 * @author Alexis Girault
 */
int ShapePopulationViewer::getNumberOfRows(int colNumber)
{
    int meshesNumber = this->widgetList->size();

    int rowNumber = meshesNumber/colNumber;
    if (meshesNumber%colNumber != 0)
    {
        rowNumber++;
    }

    return rowNumber;
}

/**
 * Add the Widgets in the layout according to their number,
 * and the number of columns
 * @brief ShapePopulationViewer::getNumberOfRows
 * @param colNumber
 * @author Alexis Girault
 */
void ShapePopulationViewer::placeWidgetInArea(int colNumber)
{
    int meshesNumber = this->widgetList->size();

    int i_col = 0;
    int i_row = 0;

    for (int i = 0; i < meshesNumber ;i++)
    {
        QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
        layout->addWidget(this->widgetList->value(i),i_row,i_col);
        if (i_col == colNumber-1)
        {
            i_col = 0;
            i_row++;
        }
        else
        {
            i_col++;
        }
    }
}


/**
 * Resizes the widgets in their column. Used when resizing the window (dockWidget),or when preserving the widget frames proportions.
 * @brief ShapePopulationViewer::resizeWidgetInArea
 * @author Alexis Girault
 */
void ShapePopulationViewer::resizeWidgetInArea()
{
    if(loaded==0) return;

    QSize QSize_dockSize = this->dockWidget->size();
    int dockWidth = QSize_dockSize.width();

    int colNumber = getNumberOfColumns();
    int rowNumber = getNumberOfRows(colNumber);
    this->scrollAreaWidgetContents->resize(dockWidth-38,(dockWidth-38)*rowNumber/colNumber);
}


// TOOLBAR FONCTIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Callback for the flip meshes menu item, this function remaps the scalars in the specified meshes to simulate a polar shift in the
 * parameterization.  No remapping of the pointdata individuals is performed, though.
 * @brief ShapePopulationViewer::flipMeshes
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::flipMeshes()
{
    if(loaded==0) return;

    //Get the numbers in a list, separated by ","
    QString QS_response = QInputDialog::getText(this,"Flip meshes","Enter the mesh numbers separated by comma");
    QStringList QS_meshIndexes = QS_response.split(",",QString::SkipEmptyParts);

    for (int i = 0; i < QS_meshIndexes.size(); i++)
    {
        QString QS_meshIndex = QS_meshIndexes.at(i);
        int meshIndex = QS_meshIndex.toInt();

        // if number of mesh incorrect
        if (meshIndex < 1 || meshIndex > this->mapperList->size())
        {
            continue;
        }

        // if no datas
        vtkFloatArray *scalars = vtkFloatArray::SafeDownCast(this->mapperList->at(meshIndex-1)->GetInput()->GetPointData()->GetScalars());
        if (scalars == NULL)
        {
            continue;
        }

        // creating the new scalar
        vtkFloatArray *newScalars = vtkFloatArray::New();
        char *scalarName = this->mapperList->at(meshIndex-1)->GetInput()->GetPointData()->GetScalars()->GetName();
        newScalars->SetName(scalarName);
        double *range = this->mapperList->at(meshIndex-1)->GetInput()->GetPointData()->GetScalars()->GetRange();
        for (int j = 0; j < scalars->GetNumberOfTuples();j++)
        {
            float scalar = scalars->GetValue(j);
            scalar = (scalar > (range[0] + range[1])/2) ? 3.0/2*range[1] - scalar: range[1]/2 - scalar;//map mid-range values to extremes, and extremes to mid-ranges
            newScalars->InsertNextValue(scalar);
        }

        // updating the scalars
        this->mapperList->at(meshIndex-1)->GetInput()->GetPointData()->SetScalars(newScalars);

        // rendering the mesh
        this->widgetList->value(meshIndex-1)->GetRenderWindow()->Render();
    }
}


/**
 * Callback to the Write Meshes menu item, this will write every current polydata back to their original files. The choice of saving each file individually with
 * a user specified file name was eliminated as it is entirely possible that very large numbers of meshes are going to be visualized, making that procedure tedious.
 * @brief ShapePopulationViewer::writeMeshes
 * @author Michael Guarino
 */
void ShapePopulationViewer::writeMeshes()
{
    if(loaded==0) return;

    QFileInfoList list = this->directory.entryInfoList();
    int meshes = 0;
    for (int i = 0; i < list.size(); i++)
    {
        QString path = list.at(i).absoluteFilePath();
        if (!path.endsWith(".vtk"))
            continue;
        QByteArray arr = path.toLatin1();
        const char *filePath = arr.data();
        vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
        writer->SetFileName(filePath);
        writer->SetInput(this->mapperList->at(meshes++)->GetInput());
        writer->Update();
    }
}


/**
 * Callback to Open .vtk Files menu item, this simply open a filedialog to let the user
 * select a directory and then calls the updateWidgets() helper function.
 * @brief ShapePopulationViewer::openVTKS
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::openVTKS()
{
    QString dir = QFileDialog::getExistingDirectory(this,tr("Open .vtk Directory"),"~",QFileDialog::ShowDirsOnly);
    QDir vtkDir(dir);
    this->directory = vtkDir;
    this->updateWidgets();
}


// VIEW OPTIONS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Callback for the View All Meshes checkbox.
 * @brief ShapePopulationViewer::on_radioButton_4_toggled
 * @param checked
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_4_toggled()
{
    this->scrollArea->setWidgetResizable(true);
}


/**
 * Callback for the View in ___ columns checkbox.  This reads from the ___ columns line edit, and then re-arranges the QVTKWidgets
 * according to the integer entry.
 * @brief ShapePopulationViewer::on_radioButton_5_toggled
 * @param checked
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_5_toggled()
{
    this->scrollArea->setWidgetResizable(false);
    resizeWidgetInArea();
}


/**
 * Callback to the ___ columns line edit.
 * A colNumberEdit modification activates column rearrangement.
 * @brief ShapePopulationViewer::on_colNumberEdit_editingFinished
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::on_colNumberEdit_editingFinished()
{
    if(loaded==0) return;

    int colNumber = getNumberOfColumns();
    placeWidgetInArea(colNumber);

    if (this->radioButton_5->isChecked() == true)
    {
        this->on_radioButton_5_toggled();
    }
    else
    {
        this->on_radioButton_4_toggled();
    }

    this->colNumberSlider->setValue(colNumber);
}

/**
 * Display the number of columns selected by the slider into colNumberEdit
 * @brief ShapePopulationViewer::on_colNumberSlider_valueChanged
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_colNumberSlider_valueChanged()
{
    printColNumber(colNumberSlider->value());
}

/**
 * Activates the column rearrangement.
 * @brief ShapePopulationViewer::on_colNumberSlider_sliderReleased
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_colNumberSlider_sliderReleased()
{
    on_colNumberEdit_editingFinished();
}


// SYNCHRONISATION ////////////////////////////////////////////////////////////////

/**
 * Callback to the REal-time Synchro Meshes radioButton.
 * Add an Observer to render meshes when modification finished
 * @brief ShapePopulationViewer::on_radioButton_1_toggled
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_1_toggled()
{
    for (int i = 0; i < this->widgetList->size(); i++)
    {
        //syncronize when rendering
        this->widgetList->value(i)->GetRenderWindow()->RemoveAllObservers();
        this->widgetList->value(i)->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, this, &ShapePopulationViewer::ModifiedHandler);
    }
}

/**

 * Callback to the Delayed Synchro Meshes radioButton.
 * Add an Observer to render meshes while modifying
 * @brief ShapePopulationViewer::on_radioButton_2_toggled
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_2_toggled()
{
    for (int i = 0; i < this->widgetList->size(); i++)
    {
        //syncronize when render modified
        this->widgetList->value(i)->GetRenderWindow()->RemoveAllObservers();
        this->widgetList->value(i)->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, this, &ShapePopulationViewer::ModifiedHandler);
    }
}

/**
 * Callback to the select all meshes checkbox.
 * Attach or detach widgets to the main camera.
 * @brief ShapePopulationViewer::on_checkBox_synchro_toggled
 * @param checked
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_checkBox_synchro_toggled(bool checked)
{
    for (int i = 0; i < this->windowList->size();i++) //reset all backgrounds
    {
        this->windowList->value(i)->GetRenderers()->GetFirstRenderer()->SetBackground(0,0,0);
        this->windowList->value(i)->Render();
    }
    this->windowList->clear(); // empty the selected windows list

    for (int i = 0; i < this->widgetList->size(); i++)
    {
        if(checked)
        {
            this->windowList->append(this->widgetList->value(i)->GetRenderWindow());//select all renderwindows
            this->windowList->value(i)->GetRenderers()->GetFirstRenderer()->SetActiveCamera(headcam);//connect to headcam for synchro

        }
        else
        {
            //Create an independant camera, copy of headcam
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
            camera->DeepCopy(headcam);
            this->widgetList->value(i)->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
        }
    }

    //Render everything
    ModifiedHandler();
}


// TEST 2 ////////////////////////////////////////////////////////////////

/**
 * Callback to the colormap dropdown menu.  This will pull the selected text from the menu, call the updateCMaps() helper, then call Render on all the
 * QVTKWidgets to render the updates.
 * @brief ShapePopulationViewer::on_colorMapBox_currentIndexChanged
 * @param arg1
 * @author Michael Guarino
 */
void ShapePopulationViewer::on_colorMapBox_currentIndexChanged()
{
    QString text = this->colorMapBox->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();

    for (int i = 0; i < this->mapperList->size(); i++)
    {
        this->mapperList->at(i)->GetInput()->GetPointData()->SetActiveScalars(cmap);

        vtkColorTransferFunction* DistanceMapTFunc = vtkColorTransferFunction::SafeDownCast(this->mapperList->at(i)->GetLookupTable());
        double *rangeLUT = this->mapperList->at(i)->GetInput()->GetPointData()->GetScalars()->GetRange();

        updateCMaps(this->mapperList->at(i), DistanceMapTFunc, rangeLUT);
    }

    this->ModifiedHandler();
}


// AXIS FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Callback to the 0 axis view button.
 * Helper function for restoring the initial distance between the meshes and the camera, and his focal point.
 * @brief ShapePopulationViewer::on_toolButton_0_clicked
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_toolButton_0_clicked()
{
    if(this->windowList->empty()) return;

    vtkRenderer* firstRenderer = this->windowList->value(0)->GetRenderers()->GetFirstRenderer();
    firstRenderer->ResetCamera();

    this->ModifiedHandler();
}

/**
 * Callback to the +Z axis view button. See viewChange() for implementation details.
 * @brief ShapePopulationViewer::on_toolButton_1_clicked
 * @author Joe Waggoner
 */
void ShapePopulationViewer::on_toolButton_1_clicked()
{
    viewChange(0,0,-1);

}
/**
 * Callback to the -Z axis view button. See viewChange() for implementation details.
 * @brief ShapePopulationViewer::on_toolButton_clicked
 * @author Joe Waggoner
 */
void ShapePopulationViewer::on_toolButton_2_clicked()
{
    viewChange(0,0,1);
}
/**
 * Callback to the +X axis view button. See viewChange() for implementation details.
 * @brief ShapePopulationViewer::on_toolButton_clicked
 * @author Joe Waggoner
 */
void ShapePopulationViewer::on_toolButton_3_clicked()
{
    viewChange(1,0,0);
}
/**
 * Callback to the -X axis view button. See viewChange() for implementation details.
 * @brief ShapePopulationViewer::on_toolButton_clicked
 * @author Joe Waggoner
 */
void ShapePopulationViewer::on_toolButton_4_clicked()
{
    viewChange(-1,0,0);
}
/**
 * Callback to the +Y axis view button. See viewChange() for implementation details.
 * @brief ShapePopulationViewer::on_toolButton_clicked
 * @author Joe Waggoner
 */
void ShapePopulationViewer::on_toolButton_5_clicked()
{
    viewChange(0,1,0);
}
/**
 * Callback to the -Y axis view button. See viewChange() for implementation details.
 * @brief ShapePopulationViewer::on_toolButton_clicked
 * @author Joe Waggoner
 */
void ShapePopulationViewer::on_toolButton_6_clicked()
{
    viewChange(0,-1,0);
}
/**
 * Helper function for viewing the meshes along a specified axis.
 * Parameters (1,0,0) will allow you to view the mesh from the end of the positive x-axis, for instance.
 * Generally, <x,y,z> is a distance-normalized vector specifying the position you want to place the viewing camera
 * (distance being the current distance from the camera to the mesh).
 * @brief ShapePopulationViewer::viewChange
 * @param x
 * @param y
 * @param z
 * @author Joe Waggoner & Alexis Girault
 */
void ShapePopulationViewer::viewChange(int x, int y, int z)
{
    if(this->windowList->empty()) return;

    vtkRenderer* firstRenderer = this->windowList->value(0)->GetRenderers()->GetFirstRenderer();
    double *coords  = firstRenderer->GetActiveCamera()->GetFocalPoint();
    double distance = firstRenderer->GetActiveCamera()->GetDistance();
    firstRenderer->GetActiveCamera()->SetPosition(coords[0]+x*distance,coords[1]+y*distance,coords[2]+z*distance);

    //setroll to .001, because it breaks on y axis if roll = 0
    firstRenderer->GetActiveCamera()->SetRoll(.001);

    this->ModifiedHandler();
}

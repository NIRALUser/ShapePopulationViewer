#include "ShapePopulationViewer.h"


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       PUBLIC FUNCTIONS                                        * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

/**
 * Constructor for ShapePopulationViewer GUI, it will initialize model vectors, connect some callbacks and also draw the arrow icons.
 * @brief ShapePopulationViewer::ShapePopulationViewer
 * @author Michael Guarino & Alexis Girault
 */
ShapePopulationViewer::ShapePopulationViewer()
{
    this->setupUi(this);

    //Vector initialization
    this->numberOfMeshes = 0;
    this->lastDirectory = "/home";
    this->headcam = vtkCamera::New();
    this->polyDataList = new QVector<vtkPolyData *>(20);
    this->rendererList = new QVector<vtkRenderer *>(20);
    this->widgetList = new QVector<QVTKWidget *>(20);
    this->selectedWindows = new QVector<vtkRenderWindow *>(20);
/*
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
*/
    // Set up action signals and slots
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->action_Write_Meshes,SIGNAL(triggered()),this,SLOT(writeMeshes()));
    connect(this->action_Open_Directory,SIGNAL(triggered()),this,SLOT(openDirectory()));
    connect(this->action_Open_Files,SIGNAL(triggered()),this,SLOT(openFiles()));
    connect(this->action_Delete_Surfaces,SIGNAL(triggered()),this,SLOT(closeAll()));
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



// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                        MENU FUNCTIONS                                         * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
/**
 * Callback to "Open Directory" menu item, this  open a filedialog to let the user
 * select a directory and then calls the updateWidgets() helper function.
 * @brief ShapePopulationViewer::openDirectory
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::openDirectory()
{

    // get directory
    QString dir = QFileDialog::getExistingDirectory(this,tr("Open .vtk Directory"),lastDirectory,QFileDialog::ShowDirsOnly);

    // if directory chosen
    if(dir!="")
    {
        // Add files in the fileList
        lastDirectory = dir;
        QDir vtkDir(dir);
        this->fileList.append(vtkDir.entryInfoList());

        // Control the files format
        for (int i = 0; i < fileList.size(); i++)
        {
            QString QFilePath = fileList.at(i).canonicalFilePath();
            if (!QFilePath.endsWith(".vtk"))
            {
                fileList.removeAt(i);
                i--;
            }
        }

        // Display widgets
        this->selectedWindows->clear();
        this->updateWidgets();
    }
}

/**
 * Callback to "Open .vtk Files" menu item, this  open a filedialog to let the user
 * select files and then calls the updateWidgets() helper function.
 * @brief ShapePopulationViewer::openFiles
 * @author Alexis Girault
 */
void ShapePopulationViewer::openFiles()
{
    QStringList stringList = QFileDialog::getOpenFileNames(this,tr("Open .vtk Files"),lastDirectory,"VTK Files (*.vtk)");

    if(stringList.size()!=0)
    {
        lastDirectory=QFileInfo(stringList.at(0)).path();

        for(int i=0; i < stringList.size(); i++)
        {
            this->fileList.append(QFileInfo(stringList.at(i)));
        }

        // Display widgets
        this->selectedWindows->clear();
        this->updateWidgets();
    }
}

/**
 * Callback to "Delete All surfaces" menu item, that calls the updateWidgets()
 * helper function after clearing the fileList.
 * @brief ShapePopulationViewer::closeAll
 * @author Alexis Girault
 */
void ShapePopulationViewer::closeAll()
{
    //clear any Content from the layout
    QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
    for (int i = 0; i < this->widgetList->size(); i++)
    {
        layout->removeWidget(this->widgetList->value(i));
        delete this->widgetList->value(i);
    }

    //Disable buttons
    axisButton->setDisabled(true);
    radioButton_realTimeSync->setDisabled(true);
    radioButton_delayedSync->setDisabled(true);
    checkBox_synchro->setDisabled(true);
    radioButton_viewAll->setDisabled(true);
    radioButton_viewSquare->setDisabled(true);
    colNumberTXT->setDisabled(true);
    colNumberEdit->setDisabled(true);
    colNumberSlider->setDisabled(true);
    colorMapBox->setDisabled(true);
    pushButton_flip->setDisabled(true);
    pushButton_delete->setDisabled(true);
    pushButton_center->setDisabled(true);

    //Initialize Menu actions
    action_Open_Directory->setText("Open directory");
    action_Open_Files->setText("Open .vtk files");
    action_Write_Meshes->setDisabled(true);
    action_Delete_Surfaces->setDisabled(true);

    //Empty the meshes FileInfo List
    this->fileList.clear();
    this->polyDataList->clear();
    this->rendererList->clear();
    this->widgetList->clear();
    this->selectedWindows->clear();
    this->numberOfMeshes = 0;
}


/**
 * Callback to the Write Meshes menu item, this will write every current polydata back to their original files. The choice of saving each file individually with
 * a user specified file name was eliminated as it is entirely possible that very large numbers of meshes are going to be visualized, making that procedure tedious.
 * @brief ShapePopulationViewer::writeMeshes
 * @author Michael Guarino
 */
void ShapePopulationViewer::writeMeshes()
{
    if(this->widgetList->size()==0) return;

    int meshes = 0;
    for (int i = 0; i < fileList.size(); i++)
    {
        QString path = fileList.at(i).absoluteFilePath();
        if (!path.endsWith(".vtk"))
            continue;
        QByteArray arr = path.toLatin1();
        const char *filePath = arr.data();
        vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
        writer->SetFileName(filePath);
        writer->SetInput(this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
        writer->Update();
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       DISPLAY FUNCTIONS                                       * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
/**
 * Helper function which reads .vtk files and renders each polydata within in a
 * separate QVTKWidget.  All model vectors are filled as well.
 * @brief ShapePopulationViewer::updateWidgets
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::updateWidgets()
{
    if(numberOfMeshes==0) //clear all vectors so they might be refilled
    {
        this->polyDataList->clear();
        this->rendererList->clear();
        this->widgetList->clear();
        this->selectedWindows->clear();
    }

    // READER
    for (int i = numberOfMeshes; i < fileList.size(); i++)
    {
        //get filepath and fileNames
        QString QFilePath = fileList.at(i).canonicalFilePath();
        QByteArray path = QFilePath.toLatin1();
        const char *filePath = path.data();

        //initialize a vtkPolyDataReader to read the .vtk file
        vtkPolyDataReader * meshReader = vtkPolyDataReader::New() ;
        meshReader->SetFileName ( filePath );
        meshReader->ReadAllScalarsOn();//make sure we are reading scalars
        meshReader->Update();//wire read setting preparation
        this->polyDataList->append(meshReader->GetOutput());//read the file : polydata is our mesh
    }

    // SMOOTHER
    for (int i = numberOfMeshes; i < polyDataList->size(); i++)
    {
        //smooth the image using a normal generator
        vtkPolyDataNormals * normalGenerator = vtkPolyDataNormals::New();
        normalGenerator->SetInput(polyDataList->value(i));
        normalGenerator->SplittingOff();
        normalGenerator->ComputePointNormalsOn();
        normalGenerator->ComputeCellNormalsOff();
        normalGenerator->Update();
        polyDataList->replace(i,normalGenerator->GetOutput());
    }

    // PIPELINE VTK VISU
    for (int i = numberOfMeshes; i < polyDataList->size(); i++)
    {
        //MAPPER
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(polyDataList->value(i)->GetProducerPort());

        //ACTOR
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        //RENDERER
        vtkRenderer * renderer = vtkRenderer::New();
        this->rendererList->append(renderer);
        renderer->AddActor(actor);
        renderer->SetActiveCamera(headcam); //set the active camera for this renderer to main camera
        renderer->ResetCamera();
    }

    // QT WIDGETS
    for (int i = numberOfMeshes; i < rendererList->size(); i++)
    {
        //QVTKWIDGET
        QVTKWidget *meshWidget = new QVTKWidget(this->scrollAreaWidgetContents);
        this->widgetList->append(meshWidget);
        meshWidget->GetRenderWindow()->AddRenderer(rendererList->value(i));

        //SELECTION
        meshWidget->GetInteractor()->AddObserver(vtkCommand::StartInteractionEvent, this, &ShapePopulationViewer::SelectWidget);
        meshWidget->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent, this, &ShapePopulationViewer::UnselectWidget);
    }

    // ANNOTATIONS
    for (int i = numberOfMeshes; i < rendererList->size(); i++)
    {
        //NAME
        QString QFileName = fileList.at(i).fileName();
        QByteArray nam = QFileName.toLatin1();
        const char *fileName = nam.data();

        //NUMBER OF POINTS
        vtkIdType numberOfPoints = polyDataList->value(i)->GetNumberOfPoints(); //number of points

        //CREATE THE STRING
        std::stringstream strs;
        strs << fileName <<std::endl
             << (int)numberOfPoints << " points" <<std::endl;
        std::string temp_str = strs.str();
        const char* txt_cornerAnnotation = temp_str.c_str();

        //CORNER
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation->SetLinearFontScaleFactor( 2 );
        cornerAnnotation->SetNonlinearFontScaleFactor( 1 );
        cornerAnnotation->SetMaximumFontSize( 15 );
        cornerAnnotation->SetText( 2,txt_cornerAnnotation);
        rendererList->value(i)->AddViewProp(cornerAnnotation);
    }

    // SCALAR BAR
    for (int i = numberOfMeshes; i < rendererList->size(); i++)
    {
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar->SetLookupTable(rendererList->value(i)->GetActors()->GetLastActor()->GetMapper()->GetLookupTable());
        scalarBar->SetNumberOfLabels(5);
        scalarBar->SetMaximumWidthInPixels(60);

        vtkSmartPointer<vtkTextProperty> LabelProperty = vtkSmartPointer<vtkTextProperty>::New();
        LabelProperty->SetFontSize(12);
        LabelProperty->ShadowOn();
        scalarBar->SetLabelTextProperty(LabelProperty);

        rendererList->value(i)->AddActor2D(scalarBar);
    }

    // COLORMAPS
    compute_colorMaps_intersection();
    colorMapBox->clear();
    colorMapBox->addItems(commonColorMaps);     // and then add them to the GUI.
    on_checkBox_synchro_toggled(true);          // Now we select all windows
    on_colorMapBox_currentIndexChanged();       // to apply the colorMap on all of them.


    //Update the number of meshes
    this->numberOfMeshes = fileList.size();

    // GUI BUTTONS
    axisButton->setDisabled(false);
    radioButton_realTimeSync->setDisabled(false);
    radioButton_delayedSync->setDisabled(false);
    checkBox_synchro->setDisabled(false);
    radioButton_viewAll->setDisabled(false);
    radioButton_viewSquare->setDisabled(false);
    colNumberTXT->setDisabled(false);
    colNumberEdit->setDisabled(false);
    colNumberSlider->setDisabled(false);
    colNumberSlider->setMaximum(fileList.size());

    // GUI ACTIONS
    action_Open_Directory->setText("Add directory");
    action_Open_Files->setText("Add .vtk files");
    action_Write_Meshes->setDisabled(true); // to do
    action_Delete_Surfaces->setDisabled(false);

    // GUI WIDGETS DISPLAY

    int colNumber = 0;
    int sum = 0;
    int nextOdd = 1;
    for ( ;sum < this->widgetList->size();colNumber++,nextOdd += 2)
    {
        sum += nextOdd;//simple integer square root, will give the ceiling of the colNumber => cols >= rows
    }
    printColNumber(colNumber+1);                //Display the number of columns in colNumberEdit,
    on_colNumberEdit_editingFinished();         //and display the Widgets according to this number.
    on_radioButton_viewAll_toggled();           //Display All surfaces,
    this->radioButton_viewAll->toggle();        //and toogle the GUI button.
    on_radioButton_delayedSync_toggled();       //Start with a delayed synchro,
    this->radioButton_delayedSync->toggle();    //and toogle the GUI button.
    on_checkBox_synchro_toggled(false);         //Start with meshes not selected,
    this->checkBox_synchro->setChecked(false);  //and uncheck the synchro checkBox.

}


/**
 * Permits to get the ID of the widget selected.
 * @brief ShapePopulationViewer::SelectWidget
 * @author Alexis Girault
 */
void ShapePopulationViewer::SelectWidget(vtkObject* selectedObject, unsigned long, void* )
{
    if(checkBox_synchro->isChecked()) return; // Don't do anything if the synchro is on "All"

    //Get the interactor used
    vtkSmartPointer<QVTKInteractor> selectedInteractor = (QVTKInteractor*)selectedObject;
    vtkSmartPointer<vtkRenderWindow> selectedWindow = selectedInteractor->GetRenderWindow();

    //if the renderwindow already is in the renderselectedWindows
    if(this->selectedWindows->contains(selectedWindow)) return;

    //if not, allow GUI actions on selections
    this->colorMapBox->setDisabled(false);
    this->pushButton_flip->setDisabled(false);
    this->pushButton_delete->setDisabled(false);
    this->pushButton_center->setDisabled(false);

    // NEW SELECTION (Ctrl not pushed)
    if(selectedInteractor->GetControlKey()==0)
    {
        for (int i = 0; i < this->selectedWindows->size();i++)                              //reset backgrounds and cameras
        {
            this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->SetBackground(0,0,0);
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
            camera->DeepCopy(this->headcam);
            this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
            this->selectedWindows->value(i)->Render();
        }
        this->selectedWindows->clear();                                                     // empty the selectedWindows list
    }

    // SELECTING
    selectedWindow->GetRenderers()->GetFirstRenderer()->SetBackground(0.1,0.0,0.3);         //Background color to grey
    if(this->selectedWindows->isEmpty())                                                    //Copy last camera if new selection
    {
        this->headcam->DeepCopy(selectedWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera());
    }
    selectedWindow->GetRenderers()->GetFirstRenderer()->SetActiveCamera(this->headcam);     //Set renderWindow to headcam
    this->selectedWindows->append(selectedWindow);                                          //Add to the selectedWindows List

    //Update Colormap
    if(selectedInteractor->GetControlKey()==1)  //to the last colormap if not first selection
    {
        on_colorMapBox_currentIndexChanged();
    }
    else                                        //or to the one in the combobox if new selection
    {
        const char * cmap = selectedWindow->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput()->GetPointData()->GetScalars()->GetName();
        int index = colorMapBox->findText(cmap);
        if (index != -1)
            colorMapBox->setCurrentIndex(index);
    }

    //if everything is selected, check Select All
    if(selectedWindows->size()==widgetList->size())
    {
        this->checkBox_synchro->setChecked(true);
    }
}


/**
 * Unselect the selected Widgets by emptying the selectedWindows.
 * @brief ShapePopulationViewer::UnselectWidget
 * @author Alexis Girault
 */
void ShapePopulationViewer::UnselectWidget(vtkObject*, unsigned long, void* voidEvent)
{
    QKeyEvent * keyEvent = (QKeyEvent*) voidEvent;
    //qDebug()<<QKeySequence(keyEvent->key()).toString();

    if((keyEvent->key() == Qt::Key_Escape))
    {
        //disallow GUI actions
        this->colorMapBox->setDisabled(true);
        this->pushButton_flip->setDisabled(true);
        this->pushButton_delete->setDisabled(true);
        this->pushButton_center->setDisabled(true);

        for (int i = 0; i < this->selectedWindows->size();i++) //reset all backgrounds and cameras
        {
            this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->SetBackground(0,0,0);
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();

            camera->DeepCopy(headcam);
            this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
            this->selectedWindows->value(i)->Render();
        }
        this->selectedWindows->clear(); // empty the selected windows list

        //Unselect "Select All"
        if(checkBox_synchro->isChecked())
        {
            this->checkBox_synchro->setChecked(false);
        }
    }

}

/**
 * Callback for the deletes meshes buttonm that delete le selection, the widget,
 * and the file from all lists, then adjust the remaining widgets.
 * @brief ShapePopulationViewer::on_pushButton_delete_clicked
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_pushButton_delete_clicked()
{
    pushButton_delete->setDisabled(true);

    // Deleting the selection, the widget, and the data
    QGridLayout *layout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
    for (int i = 0; i < this->selectedWindows->size(); i++)
    {
        for(int j = 0; j < this->widgetList->size(); j++)\
        {
            if(this->widgetList->at(j)->GetRenderWindow()==selectedWindows->at(i))
            {
                this->fileList.removeAt(j);
                this->selectedWindows->remove(i);
                this->polyDataList->remove(j);
                this->rendererList->remove(j);
                delete this->widgetList->value(j);
                this->widgetList->remove(j);
                layout->removeWidget(this->widgetList->value(j));

                i--;
                break;
            }
        }
        this->colNumberSlider->setMaximum(colNumberSlider->maximum()-1); // Readjusting in columns
    }
    this->numberOfMeshes = fileList.size();

    // If no more widgets, do as closeAll
    if(widgetList->size()==0)
    {
        closeAll();
    }
    else
    {
        on_colNumberEdit_editingFinished();

        colorMapBox->clear();
        compute_colorMaps_intersection();
        colorMapBox->addItems(commonColorMaps);     // and then add them to the GUI.
        on_checkBox_synchro_toggled(true);          // Now we select all windows
        on_colorMapBox_currentIndexChanged();       // to apply the colorMap on all of them.
        on_checkBox_synchro_toggled(false);
    }
}

/**
 * Handler to any modified event sent by a QVTKWidget in the viewport.
 * The handler calls render on all the windows provided user is viewing in synchronized mode.
 * @brief ShapePopulationViewer::ModifiedHandler
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::ModifiedHandler()
{
    for (int i = 0; i < this->selectedWindows->size();i++) //disable the renderWindows to callback ModifiedHandler again
    {
        this->selectedWindows->value(i)->RemoveAllObservers();
    }

    for (int i = 0; i < this->selectedWindows->size();i++) //render all windows selected (one of them will be the event window)
    {
        this->selectedWindows->value(i)->Render();
    }

    for (int i = 0; i < this->selectedWindows->size();i++) //attribuate the observers back to the windows the way it used to be
    {
        if(radioButton_realTimeSync->isChecked()) on_radioButton_realTimeSync_toggled();
        else on_radioButton_delayedSync_toggled();
    }
}


/**
 * Analyzes all the attributes from the meshes to keep the common ones in commonColorMaps.
 * The selection is processed using intersections.
 * @brief ShapePopulationViewer::compute_colorMaps_intersection
 * @author Alexis Girault
 */
void ShapePopulationViewer::compute_colorMaps_intersection()
{
    commonColorMaps.clear();

    QSet<QString> firstColorMaps;
    for (int i = 0; i < rendererList->size(); i++)
    {
        int numScalars = polyDataList->value(i)->GetPointData()->GetNumberOfArrays();
        if(i==0)
        {
            for (int j = 0; j < numScalars; j++)
            {
                QString scalarName(polyDataList->value(i)->GetPointData()->GetArrayName(j));
                firstColorMaps.insert(scalarName);
            }
        }
        else
        {
            QSet<QString> compareColorMaps;
            for (int j = 0; j < numScalars; j++)
            {
                QString scalarName(polyDataList->value(i)->GetPointData()->GetArrayName(j));
                compareColorMaps.insert(scalarName);
            }
            firstColorMaps = firstColorMaps.intersect(compareColorMaps);
        }
    }
    commonColorMaps = firstColorMaps.values();  // Gets the QSet values
    commonColorMaps.sort();                     // to be able to sort them,
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       PLACING FUNCTIONS                                       * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
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
    if(this->widgetList->size()==0) return;

    QSize QSize_dockSize = this->dockWidget->size();
    int dockWidth = QSize_dockSize.width();

    int colNumber = getNumberOfColumns();
    int rowNumber = getNumberOfRows(colNumber);
    this->scrollAreaWidgetContents->resize(dockWidth-38,(dockWidth-38)*rowNumber/colNumber);
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
    if (this->radioButton_viewSquare->isChecked() == true )//view square meshes
    {
        resizeWidgetInArea();
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                         VIEW OPTIONS                                          * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

/**
 * Callback for the View All Meshes checkbox.
 * @brief ShapePopulationViewer::on_radioButton_viewAll_toggled
 * @param checked
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_viewAll_toggled()
{
    this->scrollArea->setWidgetResizable(true);
}


/**
 * Callback for the View in ___ columns checkbox.  This reads from the ___ columns line edit, and then re-arranges the QVTKWidgets
 * according to the integer entry.
 * @brief ShapePopulationViewer::on_radioButton_viewSquare_toggled
 * @param checked
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_viewSquare_toggled()
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
    if(this->widgetList->size()==0) return;

    int colNumber = getNumberOfColumns();
    placeWidgetInArea(colNumber);

    if (this->radioButton_viewSquare->isChecked() == true)
    {
        this->on_radioButton_viewSquare_toggled();
    }
    else
    {
        this->on_radioButton_viewAll_toggled();
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


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                        SYNCHRONISATION                                        * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

/**
 * Callback to the Real-time Synchro Meshes radioButton.
 * Add an Observer to render meshes when modification finished
 * @brief ShapePopulationViewer::on_radioButton_realTimeSync_toggled
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_realTimeSync_toggled()
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
 * @brief ShapePopulationViewer::on_radioButton_delayedSync_toggled
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_radioButton_delayedSync_toggled()
{
    for (int i = 0; i < this->widgetList->size(); i++)
    {
        //syncronize when render modified
        this->widgetList->value(i)->GetRenderWindow()->RemoveAllObservers();
        this->widgetList->value(i)->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent, this, &ShapePopulationViewer::ModifiedHandler);
    }
}

/**
 * Callback to the "select all meshes" checkbox.
 * Attach or detach widgets to the main camera.
 * @brief ShapePopulationViewer::on_checkBox_synchro_toggled
 * @param checked
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_checkBox_synchro_toggled(bool checked)
{
    this->selectedWindows->clear(); // empty the selected windows list

    if(checked) // Select all
    {
        this->colorMapBox->setDisabled(false);
        this->pushButton_flip->setDisabled(false);
        this->pushButton_delete->setDisabled(false);
        this->pushButton_center->setDisabled(false);

        for (int i = 0; i < this->widgetList->size(); i++)
        {
            this->selectedWindows->append(this->widgetList->value(i)->GetRenderWindow()); //select all renderwindows
            this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->SetActiveCamera(headcam); //connect to headcam for synchro
            this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->SetBackground(0.1,0.0,0.3);
        }
        on_colorMapBox_currentIndexChanged(); //update the same colormap for all
    }
    else // No synchro
    {
        this->colorMapBox->setDisabled(true);
        this->pushButton_flip->setDisabled(true);
        this->pushButton_delete->setDisabled(true);
        this->pushButton_center->setDisabled(true);

        for (int i = 0; i < this->widgetList->size(); i++)
        {
            //Create an independant camera, copy of headcam
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
            camera->DeepCopy(headcam);
            this->widgetList->value(i)->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
            this->widgetList->value(i)->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(0,0,0);
            this->widgetList->value(i)->GetRenderWindow()->Render();
        }
    }

}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                           COLORMAP                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

double round_nplaces(double value, int to)
{
    int places = 1, whole = *(&value);
    for(int i = 0; i < to; i++) places *= 10;
    value -= whole; //leave decimals
    value *= places; //0.1234 -> 123.4
    value = round(value);//123.4 -> 123
    value /= places; //123 -> .123
    value += whole; //bring the whole value back
    return value;
}

/**
 * Callback to the colormap dropdown menu.  This will pull the selected text from the menu,
 * create a new color transfer function and add it to its mapper.
 * @brief ShapePopulationViewer::on_colorMapBox_currentIndexChanged
 * @author Michael Guarino & Alexis Girault
 */
void ShapePopulationViewer::on_colorMapBox_currentIndexChanged()
{
    QString text = this->colorMapBox->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();

    for (int i = 0; i < this->selectedWindows->size(); i++)
    {
        vtkMapper * mapper = this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper();
        mapper->GetInput()->GetPointData()->SetActiveScalars(cmap);

        //Range
        double *rangeLUT = mapper->GetInput()->GetPointData()->GetScalars()->GetRange();
        double range = fabs(rangeLUT[1] - rangeLUT[0]);

        //Round
        double G = round_nplaces(rangeLUT[0] - range/256, 3);
        double Y = round_nplaces(rangeLUT[0] + range/2.0, 3);
        double R = round_nplaces(rangeLUT[1] + range/256, 3);

        //LookUpTable
        vtkColorTransferFunction* DistanceMapTFunc = vtkColorTransferFunction::New();
        DistanceMapTFunc->AdjustRange(rangeLUT);
        DistanceMapTFunc->SetColorSpaceToDiverging();       //this is necessary for the color transfer function to automatically interpolate between the points we set
        DistanceMapTFunc->RemoveAllPoints();
        DistanceMapTFunc->AddRGBPoint(G, 0, 255, 0);        // Enforce the min value to be green = 0,255,0
        DistanceMapTFunc->AddRGBPoint(Y, 255, 255, 0);      // Enforce the middle of the range to be yellow = 255,255,0
        DistanceMapTFunc->AddRGBPoint(R, 255, 0, 0);        // Enforce the max value to be red = 255,0,0

        //Mapper Update
        mapper->SetLookupTable( DistanceMapTFunc );
        mapper->ScalarVisibilityOn();
        //mapper->SetScalarRange(rangeLUT[0],rangeLUT[1]);
        //mapper->SetScalarModeToUsePointData();
        //mapper->SetColorModeToMapScalars();
        mapper->Update();

        //ScalarBar Update
        vtkActor2D * oldScalarBar = this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->GetActors2D()->GetLastActor2D();
        vtkScalarBarActor * scalarBar = (vtkScalarBarActor *)oldScalarBar;
        scalarBar->SetLookupTable(DistanceMapTFunc);

    }

    ModifiedHandler();
}

/**
 * Callback for the flip meshes button this function remaps the scalars in the specified meshes to simulate a polar shift in the
 * parameterization.  No remapping of the pointdata individuals is performed, though.
 * @brief ShapePopulationViewer::on_pushButton_flip_clicked
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_pushButton_flip_clicked()
{
    if(this->selectedWindows->empty()) return;

    for (int i = 0; i < this->selectedWindows->size();i++)
    {
        //getting the scalars
        vtkFloatArray *scalars =
                vtkFloatArray::SafeDownCast(this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput()->GetPointData()->GetScalars());
        if (scalars == NULL)
        {
            continue;
        }

        //updating the scalars
        vtkFloatArray *newScalars = vtkFloatArray::New();
        newScalars->SetName(scalars->GetName());

        double *range = scalars->GetRange();
        for (int j = 0; j < scalars->GetNumberOfTuples();j++)
        {
            float scalar = scalars->GetValue(j);
            scalar = (scalar > (range[0] + range[1])/2) ? 3.0/2*range[1] - scalar: range[1]/2 - scalar;//map mid-range values to extremes, and extremes to mid-ranges
            newScalars->InsertNextValue(scalar);
        }

        // updating the scalars
        this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput()->GetPointData()->SetScalars(newScalars);
    }
    ModifiedHandler();
}


void ShapePopulationViewer::on_pushButton_center_clicked()
{
    if(this->selectedWindows->empty()) return;

    for (int i = 0; i < this->selectedWindows->size();i++)
    {
        //Get the position
        vtkActor * testActor = this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        double * position = testActor->GetPosition();
        double * center = testActor->GetCenter();

        //Update the position
        double a =position[0]-center[0];
        double b =position[1]-center[1];
        double c =position[2]-center[2];
        double newposition[3] = {a,b,c};
        testActor->SetPosition(newposition);
        testActor->SetOrigin(newposition);
        this->selectedWindows->value(i)->GetRenderers()->GetFirstRenderer()->ResetCamera();
    }
    ModifiedHandler();
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                         AXIS FUNCTIONS                                        * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

/**
 * Callback to the 0 axis view button.
 * Helper function for restoring the initial distance between the meshes and the camera, and his focal point.
 * @brief ShapePopulationViewer::on_toolButton_0_clicked
 * @author Alexis Girault
 */
void ShapePopulationViewer::on_toolButton_0_clicked()
{
    if(this->selectedWindows->empty()) return;

    vtkRenderer* firstRenderer = this->selectedWindows->value(0)->GetRenderers()->GetFirstRenderer();
    firstRenderer->ResetCamera();

    this->ModifiedHandler();
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
    if(this->selectedWindows->empty()) return;

    vtkRenderer* firstRenderer = this->selectedWindows->value(0)->GetRenderers()->GetFirstRenderer();
    double *coords  = firstRenderer->GetActiveCamera()->GetFocalPoint();
    double distance = firstRenderer->GetActiveCamera()->GetDistance();
    firstRenderer->GetActiveCamera()->SetPosition(coords[0]+x*distance,coords[1]+y*distance,coords[2]+z*distance);

    //setroll to .001, because it breaks on y axis if roll = 0
    firstRenderer->GetActiveCamera()->SetRoll(.001);

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

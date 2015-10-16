#include "ShapePopulationQT.h"


ShapePopulationQT::ShapePopulationQT()
{
    this->setupUi(this);
    
    //Intializations
    m_toolsDisplayed = true;
    m_updateOnPositionChanged = true;
    m_updateOnAttributeChanged = true;
    m_linkCoordinate = true;
    m_displayColorMap = false;
    m_firstDisplayVector = true;
    m_numberOfMeshes = 0;
    m_lastDirectory = "";
    m_colormapDirectory = "";
    m_exportDirectory = "";
    m_cameraDialog = new cameraDialogQT(this);
    m_backgroundDialog = new backgroundDialogQT(this);
    m_CSVloaderDialog = new CSVloaderQT(this);
    
    // GUI disable
    stackedWidget_ColorMapByMagnitude->show();
    stackedWidget_ColorMapByDirection->hide();
    toolBox->setDisabled(true);
    this->gradientWidget_VISU->disable();
    menuOptions->setDisabled(true);
    actionDelete->setDisabled(true);
    actionDelete_All->setDisabled(true);
    menuExport->setDisabled(true);
    
#ifdef SPV_EXTENSION
    menuExport->clear();
    menuExport->addAction("PDF");
    connect(menuExport->actions().at(0),SIGNAL(triggered()),this,SLOT(showNoExportWindow()));
#endif
    
    //Pushbuttons color
    pushButton_VISU_add->setStyleSheet("color: rgb(0, 200, 0)");
    pushButton_VISU_delete->setStyleSheet("color: rgb(200, 0, 0)");

    //Pushbuttons link
    pushButton_VISU_link_coordinate->setIcon(QIcon(":/resources/LinkOn.png"));

    
    //Checkbox icons
    std::string eyeStyleSheet;
    eyeStyleSheet += "QCheckBox::indicator:checked{ image: url(:/resources/eyeOpen.png);}";
    eyeStyleSheet += "QCheckBox::indicator:unchecked{ image: url(:/resources/eyeClosed.png);}";
    eyeStyleSheet += "QCheckBox::indicator:checked:hover{ image: url(:/resources/eyeRed.png);}";
    eyeStyleSheet += "QCheckBox::indicator:checked:pressed{ image: url(:/resources/eyeHalf.png);}";
    eyeStyleSheet += "QCheckBox::indicator:unchecked:pressed{ image: url(:/resources/eyeHalf.png);}";
    eyeStyleSheet += "QCheckBox::indicator:checked:disabled{ image: url(:/resources/eyeOpenDisabled.png);}";
    eyeStyleSheet += "QCheckBox::indicator:unchecked:disabled{ image: url(:/resources/eyeClosedDisabled.png);}";
    
    this->checkBox_displayMeshName->setStyleSheet(QString::fromStdString(eyeStyleSheet));
    this->checkBox_displayAttribute->setStyleSheet(QString::fromStdString(eyeStyleSheet));
    this->checkBox_displayColorbar->setStyleSheet(QString::fromStdString(eyeStyleSheet));
    this->checkBox_displayAxis->setStyleSheet(QString::fromStdString(eyeStyleSheet));
    this->checkBox_displaySphere->setStyleSheet(QString::fromStdString(eyeStyleSheet));
    this->checkBox_displayTitles->setStyleSheet(QString::fromStdString(eyeStyleSheet));

    // ToolTip
    this->checkBox_displayAbsoluteColorMapByDirection->setToolTip("This color map by direction has been created \nby taking the absolute value of each coordinates");
    this->radioButton_displayColorMapByDirection->setToolTip("This color map has been created by \nassociating one color at one direction");
    this->checkBox_displayAxis->setToolTip("Manage display of the marks \nof color by absolute direction");
    this->checkBox_displayTitles->setToolTip("Manage display of the titles of \nthe marks containing axes or spheres.\nThese titles indicate if the marker is used for \na color map or a color of vectors.");
    this->checkBox_displaySphere->setToolTip("Manage display of the marks \nof color by direction");


    QPalette backgroundColor = frame_DISPLAY->palette();
    backgroundColor.setColor( backgroundRole(), QColor( 255, 255, 255 ) );
    frame_DISPLAY->setPalette( backgroundColor );
    
    //Menu signals
    connect(actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(actionOpen_Directory,SIGNAL(triggered()),this,SLOT(openDirectory()));
    connect(actionOpen_VTK_Files,SIGNAL(triggered()),this,SLOT(openFiles()));
    connect(actionLoad_CSV,SIGNAL(triggered()),this,SLOT(loadCSV()));
    connect(m_CSVloaderDialog,SIGNAL(sig_itemsSelected(QFileInfoList)),this,SLOT(slot_itemsSelected(QFileInfoList)));
    connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteSelection()));
    connect(actionDelete_All,SIGNAL(triggered()),this,SLOT(deleteAll()));
    connect(actionCameraConfig,SIGNAL(triggered()),this,SLOT(showCameraConfigWindow()));
    connect(actionBackgroundConfig,SIGNAL(triggered()),this,SLOT(showBackgroundConfigWindow()));
    connect(actionLoad_Colorbar,SIGNAL(triggered()),this,SLOT(loadColorMap()));
    connect(actionSave_Colorbar,SIGNAL(triggered()),this,SLOT(saveColorMap()));
#ifndef SPV_EXTENSION
    connect(actionTo_PDF,SIGNAL(triggered()),this,SLOT(exportToPDF()));
    connect(actionTo_PS,SIGNAL(triggered()),this,SLOT(exportToPS()));
    connect(actionTo_EPS,SIGNAL(triggered()),this,SLOT(exportToEPS()));
    connect(actionTo_TEX,SIGNAL(triggered()),this,SLOT(exportToTEX()));
    connect(actionTo_SVG,SIGNAL(triggered()),this,SLOT(exportToSVG()));
#endif
    //gradView Signals
    connect(gradientWidget_VISU,SIGNAL(arrowMovedSignal(qreal)), this, SLOT(slot_gradArrow_moved(qreal)));
    connect(gradientWidget_VISU,SIGNAL(arrowSelectedSignal(qreal)), this, SLOT(slot_gradArrow_selected(qreal)));
    connect(gradientWidget_VISU,SIGNAL(arrowDoubleClickedSignal()), this, SLOT(slot_gradArrow_doubleClicked()));
    connect(gradientWidget_VISU,SIGNAL(noSelectionSignal()), this, SLOT(slot_no_gradArrow_selected()));
    
    //backgroundDialog signals
    connect(m_backgroundDialog,SIGNAL(sig_selectedColor_valueChanged(QColor)), this, SLOT(slot_selectedColor_valueChanged(QColor)));
    connect(m_backgroundDialog,SIGNAL(sig_unselectedColor_valueChanged(QColor)), this, SLOT(slot_unselectedColor_valueChanged(QColor)));
    connect(m_backgroundDialog,SIGNAL(sig_textColor_valueChanged(QColor)), this, SLOT(slot_textColor_valueChanged(QColor)));
    
    //cameraDialog signals
    connect(this,SIGNAL(sig_updateCameraConfig(cameraConfigStruct)), m_cameraDialog, SLOT(updateCameraConfig(cameraConfigStruct)));
    connect(m_cameraDialog,SIGNAL(sig_newCameraConfig(cameraConfigStruct)), this, SLOT(slot_newCameraConfig(cameraConfigStruct)));
    connect(m_cameraDialog,SIGNAL(sig_position_x_valueChanged(double)), this, SLOT(slot_position_x_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_position_y_valueChanged(double)), this, SLOT(slot_position_y_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_position_z_valueChanged(double)), this, SLOT(slot_position_z_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_focalpoint_x_valueChanged(double)), this, SLOT(slot_focalpoint_x_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_focalpoint_y_valueChanged(double)), this, SLOT(slot_focalpoint_y_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_focalpoint_z_valueChanged(double)), this, SLOT(slot_focalpoint_z_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_viewup_vx_valueChanged(double)), this, SLOT(slot_viewup_vx_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_viewup_vy_valueChanged(double)), this, SLOT(slot_viewup_vy_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_viewup_vz_valueChanged(double)), this, SLOT(slot_viewup_vz_valueChanged(double)));
    connect(m_cameraDialog,SIGNAL(sig_scale_valueChanged(double)), this, SLOT(slot_scale_valueChanged(double)));
    
    // Data Array
    QStandardItemModel * model = new QStandardItemModel(0,2,this);
    tableView->setModel(model);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    //Display
    radioButton_DISPLAY_all->toggle();                          //Display All surfaces,
    radioButton_SYNC_realtime->toggle();
}


ShapePopulationQT::~ShapePopulationQT()
{
    delete m_cameraDialog;
    delete m_backgroundDialog;
    delete m_CSVloaderDialog;
}

void ShapePopulationQT::slotExit()
{
    this->deleteAll();
    qApp->exit();
}

void ShapePopulationQT::on_pushButton_displayTools_clicked()
{
    if(m_toolsDisplayed)
    {
        this->toolBox->hide();
#ifdef WIN32
        this->pushButton_displayTools->setText(QString("^"));
#else
        this->pushButton_displayTools->setText(QString::fromUtf8("\u25B2"));
#endif
        m_toolsDisplayed = false;
    }
    else
    {
        this->toolBox->show();
#ifdef WIN32
        this->pushButton_displayTools->setText(QString("v"));
#else
        this->pushButton_displayTools->setText(QString::fromUtf8("\u25BC"));
#endif
        m_toolsDisplayed = true;
    }
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                         CLP FUNCTIONS                                         * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::loadVTKFilesCLP(QFileInfoList a_fileList)
{
    //m_fileList.append(file);                      // Add to filelist
    m_fileList.append(a_fileList);
    if(!m_fileList.isEmpty()) this->CreateWidgets();    // Display widgets
}

void ShapePopulationQT::loadCSVFileCLP(QFileInfo file)
{
    //Read .CSV with VTK
    vtkSmartPointer<vtkDelimitedTextReader> CSVreader = vtkSmartPointer<vtkDelimitedTextReader>::New();
    CSVreader->SetFieldDelimiterCharacters(",");
    CSVreader->SetFileName(file.absoluteFilePath().toStdString().c_str());
    CSVreader->SetHaveHeaders(true);
    CSVreader->Update();
    vtkSmartPointer<vtkTable> table = CSVreader->GetOutput();
    
    //Display in CSVloaderQT
    m_CSVloaderDialog->displayTable(table,file.absoluteDir());
}

void ShapePopulationQT::loadVTKDirCLP(QDir vtkDir)
{
    //Add to fileList
    m_fileList.append(vtkDir.entryInfoList());
    
    // Control the files format
    for (int i = 0; i < m_fileList.size(); i++)
    {
        QString QFilePath = m_fileList.at(i).canonicalFilePath();
        if (!QFilePath.endsWith(".vtk") && !QFilePath.endsWith(".vtp"))
        {
            m_fileList.removeAt(i);
            i--;
        }
    }
    
    // Display widgets
    if(!m_fileList.isEmpty()) this->CreateWidgets();
}

void ShapePopulationQT::loadColorMapCLP(std::string a_filePath)
{
    QString QFilePath(a_filePath.c_str());
    gradientWidget_VISU->loadColorPointList(QFilePath, &m_usedColorBar->colorPointList);
    
    this->updateColorbar_QT();
}

void ShapePopulationQT::loadCameraCLP(std::string a_filePath)
{
    m_cameraDialog->loadCameraConfig(QString(a_filePath.c_str()));
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                        MENU FUNCTIONS                                         * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::openDirectory()
{
    
    // get directory
    QString dir = QFileDialog::getExistingDirectory(this,tr("Open Directory"),m_lastDirectory,QFileDialog::ShowDirsOnly);
    if(dir.isEmpty() || !QDir(dir).exists()) return;
    
    // Add files in the fileList
    m_lastDirectory = dir;
    QDir vtkDir(dir);
    m_fileList.append(vtkDir.entryInfoList());
    
    // Control the files format
    for (int i = 0; i < m_fileList.size(); i++)
    {
        QString QFilePath = m_fileList.at(i).canonicalFilePath();
        if (!QFilePath.endsWith(".vtk") && !QFilePath.endsWith(".vtp"))
        {
            m_fileList.removeAt(i);
            i--;
        }
    }
    
    // Display widgets
    if(!m_fileList.isEmpty()) this->CreateWidgets();
}


void ShapePopulationQT::openFiles()
{
    QStringList stringList = QFileDialog::getOpenFileNames(this,tr("Open Files"),m_lastDirectory,"VTK Files (*.vtk *.vtp)");
    if(stringList.isEmpty())
    {
        return ;
    }
    
    m_lastDirectory=QFileInfo(stringList.at(0)).path();
    
    for(int i=0; i < stringList.size(); i++)
    {
        if(QFileInfo(stringList.at(i)).exists())
            this->m_fileList.append(QFileInfo(stringList.at(i)));
    }
    
    // Display widgets
    if(!m_fileList.isEmpty())
    {
        this->CreateWidgets();
    }
}


void ShapePopulationQT::loadCSV()
{
    // get directory
    QString filename = QFileDialog::getOpenFileName(this,tr("Open .csv file"),m_lastDirectory,"CSV file (*.csv)");
    if(filename.isEmpty() || !QFileInfo(filename).exists()) return;
    
    //MAJ lastDirectory
    QFileInfo file(filename);
    m_lastDirectory= file.path();
    
    //Read .CSV with VTK
    vtkSmartPointer<vtkDelimitedTextReader> CSVreader = vtkSmartPointer<vtkDelimitedTextReader>::New();
    CSVreader->SetFieldDelimiterCharacters(",");
    CSVreader->SetFileName(filename.toStdString().c_str());
    CSVreader->SetHaveHeaders(true);
    CSVreader->Update();
    vtkSmartPointer<vtkTable> table = CSVreader->GetOutput();
    
    //Display in CSVloaderQT
    m_CSVloaderDialog->displayTable(table,file.absoluteDir());
    
}


void ShapePopulationQT::slot_itemsSelected(QFileInfoList fileList)
{
    // Add files from CSV loader
    for (int i = 0; i < fileList.size(); i++)
    {
        m_fileList.append(fileList[i]);
    }
    
    // Display widgets
    if(!m_fileList.isEmpty()) this->CreateWidgets();
}

void ShapePopulationQT::deleteAll()
{
    //clear any Content from the layout
    QGridLayout *Qlayout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
    for (unsigned int i = 0; i < m_widgetList.size(); i++)
    {
        Qlayout->removeWidget(m_widgetList.at(i));
        delete m_widgetList.at(i);
        delete m_meshList.at(i);
    }
    
    //Disable buttons
    toolBox->setDisabled(true);
    gradientWidget_VISU->disable();
    actionDelete_All->setDisabled(true);
    actionDelete->setDisabled(true);
    menuExport->setDisabled(true);
    menuOptions->setDisabled(true);
    
    //Initialize Menu actions
    actionOpen_Directory->setText("Open Directory");
    actionOpen_VTK_Files->setText("Open VTK Files");
    actionLoad_CSV->setText("Load CSV File");
    
    //Empty the meshes FileInfo List
    m_fileList.clear();
    m_meshList.clear();
    m_glyphList.clear();
    m_selectedIndex.clear();
    m_windowsList.clear();
    m_widgetList.clear();
    m_numberOfMeshes = 0;
}


void ShapePopulationQT::deleteSelection()
{
    if(m_selectedIndex.size() == 0) return;
    
    this->scrollArea->setVisible(false);
    
    // Deleting the selection, the widget, and the data
    QGridLayout *Qlayout = (QGridLayout *)this->scrollAreaWidgetContents->layout();

    // delete of all axis, sphere, and titles widgets
    deleteAllWidgets();

    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        for(unsigned int j = 0; j < m_widgetList.size(); j++)
        {
            if( j == m_selectedIndex[i])
            {
                m_fileList.removeAt(j);
                
                delete m_meshList.at(j);
                m_meshList.erase(m_meshList.begin()+j);
                m_glyphList.erase(m_glyphList.begin()+j);
                
                m_selectedIndex.erase(m_selectedIndex.begin()+i);           // CAREFUL : erase i value not j value, different vector here
                for(unsigned int k = 0; k < m_selectedIndex.size() ; k++)
                {
                    if (m_selectedIndex[k] > i)  m_selectedIndex[k]-- ;     // and then decrement the upper indeces
                }
                
                m_windowsList.erase(m_windowsList.begin()+j);
                
                Qlayout->removeWidget(m_widgetList.at(j));
                delete m_widgetList.at(j);
                m_widgetList.erase(m_widgetList.begin()+j);
                
                i--;
                break;
            }
        }
    }

    m_numberOfMeshes = m_fileList.size();
    spinBox_DISPLAY_columns->setMaximum(m_numberOfMeshes);
    
    // If no more widgets, do as deleteAll
    if(m_numberOfMeshes == 0)
    {
        deleteAll();
    }
    else
    {
        ShapePopulationBase::SelectAll();
        
        computeCommonAttributes();                                                  // get the common attributes in m_commonAttributes
        comboBox_VISU_attribute->clear();                                           // clear the Attributes in the comboBox
        m_colorBarList.clear();                                                     // clear the existing colorbars
        
        m_updateOnAttributeChanged = false;
        for(unsigned int i = 0 ; i < m_commonAttributes.size() ; i++)
        {
            colorBarStruct * colorBar = new colorBarStruct;                         //new colorbar for this attribute
            gradientWidget_VISU->reset();                                           //create points
            gradientWidget_VISU->getAllColors(&colorBar->colorPointList);           //get the points into the colorbar
            this->UpdateAttribute(m_commonAttributes[i].c_str(), m_selectedIndex);  //create the range
            colorBar->range[0] = m_commonRange[0];                                  //get the range into the colorbar
            colorBar->range[1] = m_commonRange[1];
            m_colorBarList.push_back(colorBar);                                     //add the colorbar to the list
            
            comboBox_VISU_attribute->addItem(QString(m_commonAttributes[i].c_str()));   // Then add the attribute to the comboBox
        }
        m_updateOnAttributeChanged = true;
        
        this->UpdateAttribute(m_commonAttributes[0].c_str(), m_selectedIndex);
        m_usedColorBar = m_colorBarList[0];
        this->gradientWidget_VISU->setAllColors(&m_usedColorBar->colorPointList);
        spinBox_VISU_min->setValue(m_usedColorBar->range[0]);
        spinBox_VISU_max->setValue(m_usedColorBar->range[1]);
        this->updateColorbar_QT();
        this->updateArrowPosition();
        

        // Tab Vectors
        if(checkBox_displayVectors->isChecked()) checkBox_displayVectors->click();
        tab_vectors->setDisabled(true);
        // Display the color map by magnitude and disabled the color map by direction
        if (radioButton_displayColorMapByDirection->isChecked())
        {
            radioButton_displayColorMapByMagnitude->click();
        }
        radioButton_displayColorMapByDirection->setEnabled(false);


        // initialization of all axis, sphere, and titles widgets
        initializationAllWidgets();

        this->updateInfo_QT();
        on_spinBox_DISPLAY_columns_valueChanged();
    }
}




// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                     BACKGROUND FUNCTIONS                                      * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::showBackgroundConfigWindow()
{
    m_backgroundDialog->show();
}

void ShapePopulationQT::slot_selectedColor_valueChanged(QColor color)
{
    double selectedColor[3];
    
    //Colors conversion from 0-255 to 0-1
    selectedColor[0] = (double)color.red()/255.0;
    selectedColor[1] = (double)color.green()/255.0;
    selectedColor[2] = (double)color.blue()/255.0;
    
    this->setBackgroundSelectedColor(selectedColor);
}


void ShapePopulationQT::slot_unselectedColor_valueChanged(QColor color)
{
    double unselectedColor[3];
    
    //Colors conversion from 0-255 to 0-1
    unselectedColor[0] = (double)color.red()/255.0;
    unselectedColor[1] = (double)color.green()/255.0;
    unselectedColor[2] = (double)color.blue()/255.0;
    
    this->setBackgroundUnselectedColor(unselectedColor);
}


void ShapePopulationQT::slot_textColor_valueChanged(QColor color)
{
    double textColor[3];
    
    //Colors conversion from 0-255 to 0-1
    textColor[0] = (double)color.red()/255.0;
    textColor[1] = (double)color.green()/255.0;
    textColor[2] = (double)color.blue()/255.0;
    
    this->setLabelColor(textColor);
    
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       CAMERA FUNCTIONS                                        * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::showCameraConfigWindow()
{
    m_cameraDialog->show();
    this->UpdateCameraConfig();
}


void ShapePopulationQT::UpdateCameraConfig()
{
    if(m_cameraDialog->isVisible())
    {
        ShapePopulationBase::UpdateCameraConfig();
        emit sig_updateCameraConfig(m_headcamConfig);
    }
}

void ShapePopulationQT::slot_position_x_valueChanged(double arg1)
{
    double * position = m_headcam->GetPosition();
    m_headcam->SetPosition(arg1,position[1],position[2]);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_position_y_valueChanged(double arg1)
{
    double * position = m_headcam->GetPosition();
    m_headcam->SetPosition(position[0],arg1,position[2]);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_position_z_valueChanged(double arg1)
{
    double * position = m_headcam->GetPosition();
    m_headcam->SetPosition(position[0],position[1],arg1);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_focalpoint_x_valueChanged(double arg1)
{
    double * focalpoint = m_headcam->GetFocalPoint();
    m_headcam->SetFocalPoint(arg1,focalpoint[1],focalpoint[2]);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_focalpoint_y_valueChanged(double arg1)
{
    double * focalpoint = m_headcam->GetFocalPoint();
    m_headcam->SetFocalPoint(focalpoint[0],arg1,focalpoint[2]);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_focalpoint_z_valueChanged(double arg1)
{
    double * focalpoint = m_headcam->GetFocalPoint();
    m_headcam->SetFocalPoint(focalpoint[0],focalpoint[1],arg1);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_viewup_vx_valueChanged(double arg1)
{
    double * viewup = m_headcam->GetViewUp();
    m_headcam->SetViewUp(arg1,viewup[1],viewup[2]);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_viewup_vy_valueChanged(double arg1)
{
    double * viewup = m_headcam->GetViewUp();
    m_headcam->SetViewUp(viewup[0],arg1,viewup[2]);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_viewup_vz_valueChanged(double arg1)
{
    double * viewup = m_headcam->GetViewUp();
    m_headcam->SetViewUp(viewup[0],viewup[1],arg1);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}
void ShapePopulationQT::slot_scale_valueChanged(double arg1)
{
    m_headcam->SetParallelScale(arg1);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}

void ShapePopulationQT::slot_newCameraConfig(cameraConfigStruct cam)
{
    m_headcam->SetPosition(cam.pos_x,cam.pos_y,cam.pos_z);
    m_headcam->SetFocalPoint(cam.foc_x,cam.foc_y,cam.foc_z);
    m_headcam->SetViewUp(cam.view_vx,cam.view_vy,cam.view_vz);
    m_headcam->SetParallelScale(cam.scale);
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       LOAD/SAVE COLORBAR                                      * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::loadColorMap()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open .spvcm file"),m_colormapDirectory,"SPVCM file (*.spvcm)");
    if(filename == "") return;
    
    QFileInfo file(filename);
    m_colormapDirectory= file.path();
    gradientWidget_VISU->loadColorPointList(filename, &m_usedColorBar->colorPointList);
    
    this->updateColorbar_QT();
}


void ShapePopulationQT::saveColorMap()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save .spvcm file"),m_colormapDirectory,"SPVCM file (*.spvcm)");
    if(filename == "") return;
    
    QFileInfo file(filename);
    m_colormapDirectory= file.path();
    if(file.suffix() != QString("spvcm")) filename += ".spvcm";
    gradientWidget_VISU->saveColorPointList(filename);
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       CREATE WIDGETS                                          * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::CreateWidgets()
{
    this->scrollArea->setVisible(false);
    
    /* VTK WINDOWS */
    for (int i = m_numberOfMeshes; i < m_fileList.size(); i++)
    {
        //get filepath and fileNames
        QByteArray path = m_fileList[i].absoluteFilePath().toLatin1();
        const char *filePath = path.data();
        
        CreateNewWindow(filePath);
    }
    
    /* QT WIDGETS */
    for (int i = m_numberOfMeshes; i < m_fileList.size(); i++)
    {
        QVTKWidget *meshWidget = new QVTKWidget(this->scrollAreaWidgetContents);
        m_widgetList.push_back(meshWidget);
        meshWidget->GetRenderWindow()->AddRenderer(m_windowsList.at(i)->GetRenderers()->GetFirstRenderer());
        meshWidget->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, this, &ShapePopulationQT::ClickEvent);
        meshWidget->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent, this, &ShapePopulationBase::KeyPressEventVTK);
        meshWidget->GetInteractor()->AddObserver(vtkCommand::ModifiedEvent, this, &ShapePopulationBase::CameraChangedEventVTK);
        meshWidget->GetInteractor()->AddObserver(vtkCommand::StartInteractionEvent, this, &ShapePopulationBase::StartEventVTK);
        meshWidget->GetInteractor()->AddObserver(vtkCommand::EndInteractionEvent, this, &ShapePopulationBase::EndEventVTK);
    }

    /* WINDOWS */
    m_windowsList.clear();    
    for (unsigned int i = 0; i < m_widgetList.size(); i++)
    {
        m_windowsList.push_back(m_widgetList.at(i)->GetRenderWindow());
    }
    RealTimeRenderSynchro(radioButton_SYNC_realtime->isChecked());              //Start with a realtime synchro

    /* AXIS and SPHERE WIDGETS */
    deleteAllWidgets(); // delete of all widgets (axis, sphere, titles of these widgets)
    initializationAllWidgets(); // initialization of all the widgets

    /* ATTRIBUTES & COLORBARS */
    ShapePopulationBase::SelectAll();

    m_displayColorMapByMagnitude.clear();
    m_displayColorMapByDirection.clear();
    m_displayAbsoluteColorMapByDirection.clear();
    m_displayVectors.clear();
    m_displayVectorsByMagnitude.clear();
    m_displayVectorsByDirection.clear();
    m_displayVectorsByAbsoluteDirection.clear();
    for (unsigned int i = 0; i < m_widgetList.size(); i++)
    {
        m_displayColorMapByMagnitude.push_back(false);
        m_displayColorMapByDirection.push_back(false);
        m_displayAbsoluteColorMapByDirection.push_back(false);
        m_displayVectors.push_back(false);
        m_displayVectorsByMagnitude.push_back(false);
        m_displayVectorsByDirection.push_back(false);
        m_displayVectorsByAbsoluteDirection.push_back(false);
    }

    computeCommonAttributes();                                                  // get the common attributes in m_commonAttributes
    comboBox_VISU_attribute->clear();                                           // clear the Attributes in the comboBox
    m_colorBarList.clear();                                                     // clear the existing colorbars
    
    m_updateOnAttributeChanged = false;
    m_noUpdateVectorsByDirection = true;
    m_valueDirectionColorMapList.clear();
    
    
    for(unsigned int i = 0 ; i < m_commonAttributes.size() ; i++)
    {
        colorBarStruct * colorBar = new colorBarStruct;                         //new colorbar for this attribute
        gradientWidget_VISU->reset();                                           //create points
        gradientWidget_VISU->getAllColors(&colorBar->colorPointList);           //get the points into the colorbar
        this->UpdateAttribute(m_commonAttributes[i].c_str(), m_selectedIndex);  //create the range
        colorBar->range[0] = m_commonRange[0];                                  //get the range into the colorbar
        colorBar->range[1] = m_commonRange[1];
        m_colorBarList.push_back(colorBar);                                     //add the colorbar to the list
        
        comboBox_VISU_attribute->addItem(QString(m_commonAttributes[i].c_str()));   // Then add the attribute to the comboBox
        
        // color map by direction
        int dimension = m_meshList[0]->GetPolyData()->GetPointData()->GetScalars(m_commonAttributes[i].c_str())->GetNumberOfComponents();
        valueDirectionColorMapStruct * valueDirectionColorMap = new valueDirectionColorMapStruct;
        if (dimension == 3 )
        {
            this->computeNorm(m_commonAttributes[i].c_str());
            this->computeRangeDirection(m_commonAttributes[i].c_str());
            for(int cc = 0; cc < 3; cc++)
            {
                valueDirectionColorMap->min[cc] = m_commonMin[cc];
                valueDirectionColorMap->max[cc] = m_commonMax[cc];
                valueDirectionColorMap->minAbs[cc] = m_commonMinAbs[cc];
                valueDirectionColorMap->maxAbs[cc] = m_commonMaxAbs[cc];
            }

            valueDirectionColorMap->norm = m_norm;
        }
        else if (dimension == 1 )
        {
            for(int cc = 0; cc < 3; cc++)
            {
                valueDirectionColorMap->min[cc] = 0.0;
                valueDirectionColorMap->max[cc] = 0.0;
                valueDirectionColorMap->minAbs[cc] = 0.0;
                valueDirectionColorMap->maxAbs[cc] = 0.0;
            }
            valueDirectionColorMap->norm = 0.0;
        }
        
        m_valueDirectionColorMapList.push_back(valueDirectionColorMap);
        
        if(dimension == 3 )
        {
            this->UpdateColorMapByDirection(m_commonAttributes[i].c_str(),i);
            this->UpdateColorMapByAbsoluteDirection(m_commonAttributes[i].c_str(),i);
        }
        
    }
    m_updateOnAttributeChanged = true;
    
    /* RENDER WINDOWS */
    this->UpdateAttribute(m_commonAttributes[0].c_str(), m_selectedIndex);
    m_usedColorBar = m_colorBarList[0];
    this->gradientWidget_VISU->setAllColors(&m_usedColorBar->colorPointList);
    spinBox_VISU_min->setValue(m_usedColorBar->range[0]);
    spinBox_VISU_max->setValue(m_usedColorBar->range[1]);
    this->updateColorbar_QT();
    this->updateArrowPosition();
    
    /* VECTORS UPDATE */
    this->setMeshOpacity((double)this->spinbox_meshOpacity->value()/100.0);
    this->setVectorScale((double)this->spinbox_vectorScale->value()/100.0);
    this->setVectorDensity(this->spinbox_arrowDens->value());
    
    m_numberOfMeshes = m_fileList.size();
    spinBox_DISPLAY_columns->setMaximum(m_numberOfMeshes);
    
    /* CHECK ALIGNMENT */
    on_comboBox_alignment_currentIndexChanged();
    
    /* GUI BUTTONS & ACTIONS */
    this->toolBox->setEnabled(true);
    if (checkBox_displayVectors->isChecked()) checkBox_displayVectors->click();
    if (radioButton_displayColorMapByDirection->isChecked())
    {
        if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]]) checkBox_displayAbsoluteColorMapByDirection->click();
    }
    else
    {
        this->displayColorMapByMagnitude(true);
        stackedWidget_ColorMapByMagnitude->show();
        stackedWidget_ColorMapByDirection->hide();
        this->RenderAll();
    }
    radioButton_displayColorMapByMagnitude->click();


    /* Options enabled or not for Vectors */
    const char * cmap = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
    int dimension = m_meshList[0]->GetPolyData()->GetPointData()->GetScalars(cmap)->GetNumberOfComponents();
    if (dimension == 1)
    {
        tab_vectors->setDisabled(true);
        radioButton_displayColorMapByDirection->setDisabled(true);
    }
    else
    {
        tab_vectors->setEnabled(true);
        widget_VISU_colorVectors->setDisabled(true);
        widget_VISU_optionVectors->setDisabled(true);
    }
    /* */

    
    this->gradientWidget_VISU->enable(&m_usedColorBar->colorPointList);
    this->menuOptions->setEnabled(true);
    this->actionDelete->setEnabled(true);
    this->actionDelete_All->setEnabled(true);
    this->menuExport->setEnabled(true);
    this->actionOpen_Directory->setText("Add Directory");
    this->actionOpen_VTK_Files->setText("Add VTK files");
    this->actionLoad_CSV->setText("Add CSV file");\
    
    /* DISPLAY INFOS */
    this->updateInfo_QT();
    
    /* GUI WIDGETS DISPLAY */
    unsigned int sum = 0;
    int colNumber = 0;
    int nextOdd = 1;
    for ( ; sum < m_numberOfMeshes ; colNumber++ , nextOdd += 2)
    {
        sum += nextOdd;                                         //simple integer square root, will give the ceiling of the colNumber => cols >= rows
    }
    spinBox_DISPLAY_columns->setValue(colNumber+1);             //Display the number of columns in spinBox_DISPLAY_columns,
    on_spinBox_DISPLAY_columns_valueChanged();                  //and display the Widgets according to this number.

    m_noUpdateVectorsByDirection = false;

}



// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                          SELECTION                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::ClickEvent(vtkObject* a_selectedObject, unsigned long notUseduLong, void* notUsedVoid)
{
    //Get the interactor used
    vtkSmartPointer<QVTKInteractor> selectedInteractor = vtkSmartPointer<QVTKInteractor>::New();
    selectedInteractor = (QVTKInteractor*)a_selectedObject;
    vtkSmartPointer<vtkRenderWindow> selectedWindow = selectedInteractor->GetRenderWindow();
    unsigned int index = getSelectedIndex(selectedWindow);
    
    //if the renderwindow already is in the renderselectedWindows...
    if( (std::find(m_selectedIndex.begin(), m_selectedIndex.end(), index)) != (m_selectedIndex.end()) )
    {
        if(selectedInteractor->GetControlKey() == 0) return; // ...and is not being unselected : quit
    }

    /* VTK SELECTION */
    ShapePopulationBase::ClickEvent(a_selectedObject,notUseduLong,notUsedVoid);
    

    if(m_selectedIndex.size() == 0)
    {
        /* DISABLE GUI ACTIONS */
        this->actionDelete->setDisabled(true);
        this->menuExport->setDisabled(true);
        this->groupBox_VIEW->setDisabled(true);
        this->groupBox_VISU->setDisabled(true);
        this->gradientWidget_VISU->disable();
        this->tabWidget->setDisabled(true);
    }
    else
    {
        /* ENABLE GUI ACTIONS */
        this->actionDelete->setEnabled(true);
        this->menuExport->setEnabled(true);
        this->groupBox_VIEW->setEnabled(true);
        this->groupBox_VISU->setEnabled(true);
        
        const char * cmap = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        int dim = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars(cmap)->GetNumberOfComponents();
        
        std::string new_cmap = std::string(cmap);
        size_t found = new_cmap.rfind("_mag");
        new_cmap = new_cmap.substr(0,found);
        if( (new_cmap !=std::string(cmap)) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()))
            dim = 3;

        if (dim == 1)
        {
            // Tab Vectors
            tab_vectors->setDisabled(true);
            // ColorMap by direction
            if (radioButton_displayColorMapByDirection->isChecked()) radioButton_displayColorMapByMagnitude->click();
            radioButton_displayColorMapByDirection->setEnabled(false);
        }
        else
        {
            // Tab Vectors
            tab_vectors->setEnabled(true);
            if(!checkBox_displayVectors->isChecked())
            {
                widget_VISU_colorVectors->setDisabled(true);
                widget_VISU_optionVectors->setDisabled(true);
            }
            // radio button of the color map by direction
            radioButton_displayColorMapByDirection->setEnabled(true);
        }
        this->gradientWidget_VISU->enable(&m_usedColorBar->colorPointList);
        this->tabWidget->setEnabled(true);
        
        // creation of color maps by direction
        m_valueDirectionColorMapList.clear();
        for(unsigned int k = 0 ; k < m_commonAttributes.size() ; k++)
        {
            int dimension = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars(m_commonAttributes[k].c_str())->GetNumberOfComponents();
            if(dimension == 3 )
            {
                this->UpdateColorMapByDirection(m_commonAttributes[k].c_str(),k);
                this->UpdateColorMapByAbsoluteDirection(m_commonAttributes[k].c_str(),k);
            }
        }
        
        /* DISPLAY INFOS */
        this->updateInfo_QT();
        this->updateAttribute_QT();


        /* UPDATE COLOR MAPS and VECTORS and BUTTONS*/
        if(dim == 1)
        {
            deleteAllWidgets();
            initializationAllWidgets();
            m_displayColorMapByDirection[index] = false;
            m_displayAbsoluteColorMapByDirection[index] = false;
            m_displayColorMapByMagnitude[index] = true;
            m_displayVectors[index] = false;
            m_displayVectorsByMagnitude[index] = false;
            m_displayVectorsByDirection[index] = false;
            m_displayVectorsByAbsoluteDirection[index] = false;
            // Show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[index]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(m_displayColorbar) scalarBar->SetVisibility(1);

        }
        else if(dim == 3)
        {
            if( (std::find(m_selectedIndex.begin(), m_selectedIndex.end(), index)) != (m_selectedIndex.end()) ) // new selection
            {
                if(m_selectedIndex.size() > 1 )
                {
                    if(selectedInteractor->GetControlKey() == 1)  // Ctrl pushed
                    {
                        // Update the scale and density of vectors by direction to the first selected window position
                        this->setVectorDensity(this->spinbox_arrowDens->value());
                        this->setVectorScale((double)this->spinbox_vectorScale->value()/100);

                        // COLOR MAP
                        // Update the color map by magnitude to the first selected window position
                        if(m_displayColorMapByMagnitude[m_selectedIndex[0]])
                        {
                            m_displayColorMapByDirection[index] = false;
                            m_displayAbsoluteColorMapByDirection[index] = false;
                            this->displayColorMapByMagnitude(true);

                        }
                        // Update the color map by direction to the first selected window position
                        else if(m_displayColorMapByDirection[m_selectedIndex[0]])
                        {
                            m_displayColorMapByMagnitude[index] = false;
                            m_displayAbsoluteColorMapByDirection[index] = false;
                            this->displayColorMapByDirection(true);
                        }
                        // Update the color map by absolute direction to the first selected window position
                        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
                        {
                            m_displayColorMapByMagnitude[index] = false;
                            m_displayAbsoluteColorMapByDirection[index] = false;
                            this->displayAbsoluteColorMapByDirection(true);
                        }
                        // VECTORS
                        if(m_displayVectors[m_selectedIndex[0]])
                        {
                            m_displayVectors[index] = true;
                            // Update the color of vectors by magnitude to the first selected window position
                            if(m_displayVectorsByMagnitude[m_selectedIndex[0]])
                            {
                                m_displayVectorsByDirection[index] = false;
                                m_displayVectorsByAbsoluteDirection[index] = false;
                                this->displayVectorsByMagnitude(true);
                            }
                            // Update the color of vectors by direction to the first selected window position
                            else if(m_displayVectorsByDirection[m_selectedIndex[0]])
                            {
                                m_displayVectorsByMagnitude[index] = false;
                                m_displayVectorsByAbsoluteDirection[index] = false;
                                this->displayVectorsByDirection(true);
                            }
                            // Update the color of vectors by absolute direction to the first selected window position
                            else if(m_displayVectorsByAbsoluteDirection[m_selectedIndex[0]])
                            {
                                m_displayVectorsByDirection[index] = false;
                                m_displayVectorsByMagnitude[index] = false;
                                this->displayVectorsByAbsoluteDirection(true);
                            }
                        }
                        else
                        {
                            m_displayVectors[index] = false;
                            m_displayVectorsByMagnitude[index] = m_displayVectorsByMagnitude[m_selectedIndex[0]];
                            m_displayVectorsByDirection[index] = m_displayVectorsByDirection[m_selectedIndex[0]];
                            m_displayVectorsByAbsoluteDirection[index] = m_displayVectorsByAbsoluteDirection[m_selectedIndex[0]];
                            this->displayVectors(false);
                        }
                    }
                }

                // Update the buttons selected
                else if(m_selectedIndex.size() == 1)
                {
                    if(selectedInteractor->GetControlKey() == 0)  // Ctrl not pushed
                    {
                        // Update the button selected according the color map
                        if (m_displayColorMapByMagnitude[index])
                        {
                            radioButton_displayColorMapByMagnitude->click();
                        }
                        else if (m_displayColorMapByDirection[index])
                        {
                            if(checkBox_displayAbsoluteColorMapByDirection->isChecked()) checkBox_displayAbsoluteColorMapByDirection->click();
                            radioButton_displayColorMapByDirection->click();
                        }
                        else if (m_displayAbsoluteColorMapByDirection[index])
                        {
                            radioButton_displayColorMapByDirection->click();
                            if(!checkBox_displayAbsoluteColorMapByDirection->isChecked()) checkBox_displayAbsoluteColorMapByDirection->click();
                        }

                        // Update the button selected according the color of vectors
                        if(!m_displayVectors[index])
                        {
                            if(checkBox_displayVectors->isChecked()) checkBox_displayVectors->click();
                        }
                        else
                        {
                            if(!checkBox_displayVectors->isChecked()) checkBox_displayVectors->click();
                            if(m_displayVectorsByMagnitude[index])
                            {
                                radioButton_displayVectorsbyMagnitude->click();
                            }
                            else if(m_displayVectorsByDirection[index])
                            {
                                radioButton_displayVectorsbyDirection->click();
                            }
                            else if(m_displayVectorsByAbsoluteDirection[index])
                            {
                                radioButton_displayVectorsByAbsoluteDirection->click();
                            }
                        }
                    }
                }
            }
        }
    }
}

void ShapePopulationQT::SelectAll()
{
    // if everything already selected
    if(m_selectedIndex.size() == m_windowsList.size()) return;
    
    ShapePopulationBase::SelectAll();
    
    
    /* ENABLE GUI ACTIONS */
    this->actionDelete->setEnabled(true);
    this->menuExport->setEnabled(true);
    this->groupBox_VIEW->setEnabled(true);
    this->groupBox_VISU->setEnabled(true);
    this->gradientWidget_VISU->enable(&m_usedColorBar->colorPointList);
    this->tabWidget->setEnabled(true);
    
    /* UPDATE WINDOWS */
    
    // Get Attribute in ComboBox
    QString text = this->comboBox_VISU_attribute->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();
    
    // Update color with this attribute
    this->UpdateAttribute(cmap, m_selectedIndex);
    this->setVectorDensity(this->spinbox_arrowDens->value());
    this->UpdateColorMap(m_selectedIndex);
    
    // Render
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
    
    /* DISPLAY INFOS */
    this->updateInfo_QT();
}


void ShapePopulationQT::UnselectAll()
{
    // if nothing already selected
    if(m_selectedIndex.size() == 0) return;
    
    ShapePopulationBase::UnselectAll();
    
    /* DISABLE GUI ACTIONS */
    this->actionDelete->setDisabled(true);
    this->menuExport->setDisabled(true);
    this->groupBox_VIEW->setDisabled(true);
    this->groupBox_VISU->setDisabled(true);
    this->gradientWidget_VISU->disable();
    this->tabWidget->setDisabled(true);
}

void ShapePopulationQT::keyPressEvent(QKeyEvent * keyEvent)
{
    //UNSELECTING
    if((keyEvent->key() == Qt::Key_Escape))
    {
        UnselectAll();
    }
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       PLACING FUNCTIONS                                       * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

int ShapePopulationQT::getNumberOfColumns()
{
    QString QStr_colNumber = this->spinBox_DISPLAY_columns->text();
    unsigned int colNumber = QStr_colNumber.toInt();
    
    if(colNumber > m_numberOfMeshes)
    {
        colNumber = m_numberOfMeshes;
        spinBox_DISPLAY_columns->setValue(colNumber);
    }
    else if(colNumber <= 0)
    {
        colNumber = 1;
        spinBox_DISPLAY_columns->setValue(colNumber);
    }
    return colNumber;
}


int ShapePopulationQT::getNumberOfRows(unsigned int colNumber)
{
    unsigned int rowNumber = m_numberOfMeshes/colNumber;
    if (m_numberOfMeshes%colNumber != 0)
    {
        rowNumber++;
    }
    return rowNumber;
}


void ShapePopulationQT::placeWidgetInArea(unsigned int colNumber)
{
    unsigned int i_col = 0;
    unsigned int i_row = 0;
    
    for (unsigned int i = 0; i < m_numberOfMeshes ;i++)
    {
        QGridLayout *Qlayout = (QGridLayout *)this->scrollAreaWidgetContents->layout();
        Qlayout->addWidget(m_widgetList.at(i),i_row,i_col);
        
        if (i_col == colNumber-1)
        {
            i_col = 0;
            i_row++;
        }
        else i_col++;
    }
}


void ShapePopulationQT::resizeWidgetInArea()
{
    if(m_numberOfMeshes == 0) return;
    
    QSize QSize_scrollArea = this->scrollAreaWidgetContents->size();
    int scrollAreaWidth = QSize_scrollArea.width();
    
    int colNumber = getNumberOfColumns();
    int rowNumber = getNumberOfRows(colNumber);
    this->scrollAreaWidgetContents->resize(scrollAreaWidth,(scrollAreaWidth)*rowNumber/colNumber);
}


void ShapePopulationQT::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        int tableWidth = tableView->width();
        int column0Width = tableView->columnWidth(0);
        int column1Width = tableView->columnWidth(1);
        int column2width = tableWidth - column0Width - column1Width;
        
        if(m_commonAttributes.size()<6) tableView->setColumnWidth(2,column2width -5);
        else tableView->setColumnWidth(2,column2width-20);
    }
}

void ShapePopulationQT::resizeEvent(QResizeEvent *Qevent)
{
    //Resizing Windows
    QMainWindow::resizeEvent(Qevent);
    
    //According to the View Options
    if (this->radioButton_DISPLAY_square->isChecked() == true )//view square meshes
    {
        resizeWidgetInArea();
    }
    
    //data range column
    on_tabWidget_currentChanged(1);
}

void ShapePopulationQT::dragEnterEvent(QDragEnterEvent *Qevent)
{
    Qevent->accept();
}

void ShapePopulationQT::dropEvent(QDropEvent* Qevent)
{
    const QMimeData* mimeData = Qevent->mimeData();
    
    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        bool load = false;
        QFileInfoList fileList;
        
        // extract the local paths of the files
        for (int i = 0; i < urlList.size(); ++i)
        {
            QString filePath = urlList.at(i).toLocalFile();
            if((filePath.endsWith(".vtk") || filePath.endsWith(".vtp")) && QFileInfo(filePath).exists())
            {
                fileList.append(QFileInfo(filePath));
                load = true;
            }
            else if(filePath.endsWith(".csv") && QFileInfo(filePath).exists())
            {
                this->loadCSVFileCLP(QFileInfo(filePath));
            }
            else if(filePath.endsWith(".spvcm") && m_numberOfMeshes > 0 && QFileInfo(filePath).exists())
            {
                loadColorMapCLP(filePath.toStdString());
            }
            else if(filePath.endsWith(".pvcc") && m_numberOfMeshes > 0 && QFileInfo(filePath).exists())
            {
                loadCameraCLP(filePath.toStdString());
            }
            else if(QDir(filePath).exists())
            {
                this->loadVTKDirCLP(QDir(filePath));
            }
        }
        if(load == true)
        {
            this->loadVTKFilesCLP(fileList);
        }
    }
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                       DISPLAY OPTIONS                                         * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //


void ShapePopulationQT::on_radioButton_DISPLAY_all_toggled()
{
    this->scrollArea->setWidgetResizable(true);
}


void ShapePopulationQT::on_radioButton_DISPLAY_square_toggled()
{
    this->scrollArea->setWidgetResizable(false);
    resizeWidgetInArea();
}

void ShapePopulationQT::on_spinBox_DISPLAY_columns_valueChanged()
{
    if(m_numberOfMeshes == 0) return;
    this->scrollArea->setVisible(false);
    
    placeWidgetInArea(spinBox_DISPLAY_columns->value());
    if (radioButton_DISPLAY_square->isChecked()) resizeWidgetInArea();
    
    this->scrollArea->setVisible(true);
}

void ShapePopulationQT::on_checkBox_displayColorbar_toggled(bool checked)
{
    this->displayColorbar(checked);
    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displayAttribute_toggled(bool checked)
{
    this->displayAttribute(checked);
    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displayMeshName_toggled(bool checked)
{
    this->displayMeshName(checked);
    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displayAxis_toggled(bool checked)
{
    this->displayAxis(checked);
    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displaySphere_toggled(bool checked)
{
    this->displaySphere(checked);
    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displayTitles_toggled(bool checked)
{
    this->displayTitles(checked);
    this->RenderAll();
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                        SYNCHRONISATION                                        * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //


void ShapePopulationQT::on_radioButton_SYNC_realtime_toggled()
{
    RealTimeRenderSynchro(true);
}

void ShapePopulationQT::on_radioButton_SYNC_delayed_toggled()
{
    RealTimeRenderSynchro(false);
}

void ShapePopulationQT::on_pushButton_SYNC_all_clicked()
{
    SelectAll();
}
void ShapePopulationQT::on_pushButton_SYNC_unselect_clicked()
{
    UnselectAll();
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                         VIEW FUNCTIONS                                        * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::on_pushButton_VIEW_reset_clicked()
{
    if(m_selectedIndex.size() == 0) return;
    
    this->ResetHeadcam();
    
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}

void ShapePopulationQT::on_toolButton_VIEW_P_clicked() {ChangeView(0,0,-1);}

void ShapePopulationQT::on_toolButton_VIEW_A_clicked() {ChangeView(0,0,1);}

void ShapePopulationQT::on_toolButton_VIEW_L_clicked() {ChangeView(1,0,0);}

void ShapePopulationQT::on_toolButton_VIEW_R_clicked() {ChangeView(-1,0,0);}

void ShapePopulationQT::on_toolButton_VIEW_S_clicked() {ChangeView(0,1,0);}

void ShapePopulationQT::on_toolButton_VIEW_I_clicked() {ChangeView(0,-1,0);}

void ShapePopulationQT::on_comboBox_alignment_currentIndexChanged()
{
    // Get Attribute in ComboBox
    int alignment = this->comboBox_alignment->currentIndex();
    
    if(alignment == 0) this->AlignMesh(false);
    else if(alignment == 1) this->AlignMesh(true);
    
    this->RenderAll();
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                     ATTRIBUTES COLORMAP                                       * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::on_comboBox_VISU_attribute_currentIndexChanged()
{
    if(m_selectedIndex.size() == 0 || m_updateOnAttributeChanged == false) return;
    
    int index = this->comboBox_VISU_attribute->currentIndex();
    if (index != -1)
    {
        QString text = this->comboBox_VISU_attribute->currentText();
        QByteArray arr = text.toLatin1();
        const char *cmap  = arr.data();
        
        // Update Vectors
        int dimension = m_meshList[0]->GetPolyData()->GetPointData()->GetScalars(cmap)->GetNumberOfComponents();
        if (dimension == 3)
        {
            this->setVectorDensity(this->spinbox_arrowDens->value());
        }

        // Update Attribute and commonRange
        this->UpdateAttribute(cmap, m_selectedIndex);

        // Change the colorbar selected
        m_usedColorBar = m_colorBarList[index]; //the colorbar depends of the attribute
        this->gradientWidget_VISU->setAllColors(&m_usedColorBar->colorPointList);
        spinBox_VISU_min->setValue(m_usedColorBar->range[0]);
        spinBox_VISU_max->setValue(m_usedColorBar->range[1]);
        
        // Display colormap
        this->UpdateColorMap(m_selectedIndex);
        this->updateArrowPosition();

        /* Options enabled or not for Vectors */
        if (dimension == 1)
        {
            // Tab Vectors
            if(checkBox_displayVectors->isChecked()) checkBox_displayVectors->click();
            tab_vectors->setDisabled(true);
            // ColorMap by direction
            if (radioButton_displayColorMapByDirection->isChecked())
            {
                radioButton_displayColorMapByMagnitude->click();
            }
            radioButton_displayColorMapByDirection->setEnabled(false);
        }
        else if(dimension == 3)
        {
            // Tab Vectors
            tab_vectors->setEnabled(true);
            if(!checkBox_displayVectors->isChecked())
            {
                widget_VISU_colorVectors->setDisabled(true);
                widget_VISU_optionVectors->setDisabled(true);
            }
            // ColorMap by direction
            radioButton_displayColorMapByDirection->setEnabled(true);

            // Update the Range of the color map by direction
            m_usedValueDirectionColorMap = m_valueDirectionColorMapList[index];
            m_displayColorMap = true;
            if(m_displayColorMapByDirection[m_selectedIndex[0]] && !m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
            {
                spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
                spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);
                spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
                spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[2]);
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[2]);

            }
            else if (m_displayColorMapByDirection[m_selectedIndex[0]] && m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
            {
                spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->minAbs[0]);
                spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);
                spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->minAbs[1]);
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);
                spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->minAbs[2]);
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
            }

            m_displayColorMap = false;
        }

        // Render
        m_renderAllSelection = true;
        this->RenderSelection();
        m_renderAllSelection = false;
    }
}

void ShapePopulationQT::updateColorbar_QT()
{
    // Update m_colorPointList from colorbar
    gradientWidget_VISU->getAllColors(&m_usedColorBar->colorPointList);
    
    // Get Attribute in ComboBox
    QString text = this->comboBox_VISU_attribute->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();
    std::ostringstream strs;
    strs.str(""); strs.clear();
    strs << cmap << "_mag"<<std::endl;
    std::string cmap_mag = strs.str();
    
    //Get the windows with this attribute
    std::vector<unsigned int > windowsIndex;
    for(unsigned int i = 0 ; i < m_selectedIndex.size() ; i++)
    {
        const char * thisCmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        if(std::string(thisCmap) == std::string(cmap) || std::string(thisCmap) == cmap_mag)
        {
            windowsIndex.push_back(m_selectedIndex[i]);
        }
    }
    
    //Updating this "sameattributewindows" list colormap
    this->UpdateColorMap(windowsIndex);
    
    //Rendering those windows...
    for(unsigned int i = 0 ; i < windowsIndex.size() ; i++)
    {
        m_windowsList[windowsIndex[i]]->Render();
    }
}


void ShapePopulationQT::UpdateColorMapByDirection_QT()
{
    // Get Attribute in ComboBox
    int index = this->comboBox_VISU_attribute->currentIndex();
    QString text = this->comboBox_VISU_attribute->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();
    std::ostringstream strs;
    strs.str(""); strs.clear();
    strs << cmap << "_ColorByDirection"<<std::endl;
    std::string cmap_dir = strs.str();
    
    //Get the windows with this attribute
    std::vector<unsigned int > windowsIndex;
    for(unsigned int i = 0 ; i < m_selectedIndex.size() ; i++)
    {
        const char * thisCmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        if(std::string(thisCmap) == std::string(cmap) || std::string(thisCmap) == cmap_dir)
        {
            windowsIndex.push_back(m_selectedIndex[i]);
        }
    }
    
    //Updating this "sameattributewindows" list colormap
    this->UpdateColorMapByDirection(cmap,index);


    for(unsigned int i = 0 ; i < windowsIndex.size() ; i++)
    {
        //Update the color of vectors
        if(m_displayVectorsByDirection[m_selectedIndex[i]] || m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();

        //Rendering those windows...
        m_windowsList[windowsIndex[i]]->Render();
    }
}


void ShapePopulationQT::UpdateColorMapByAbsoluteDirection_QT()
{
    // Get Attribute in ComboBox
    int index = this->comboBox_VISU_attribute->currentIndex();
    QString text = this->comboBox_VISU_attribute->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();
    std::ostringstream strs;
    strs.str(""); strs.clear();
    strs << cmap << "_AbsoluteColorByDirection"<<std::endl;
    std::string cmap_abs = strs.str();
    
    //Get the windows with this attribute
    std::vector<unsigned int > windowsIndex;
    for(unsigned int i = 0 ; i < m_selectedIndex.size() ; i++)
    {
        const char * thisCmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        if(std::string(thisCmap) == std::string(cmap) || std::string(thisCmap) == cmap_abs)
        {
            windowsIndex.push_back(m_selectedIndex[i]);
        }
    }
    
    //Updating this "sameattributewindows" list colormap
    this->UpdateColorMapByAbsoluteDirection(cmap,index);

    
    for(unsigned int i = 0 ; i < windowsIndex.size() ; i++)
    {
        //Update the color of vectors
        if(m_displayVectorsByDirection[m_selectedIndex[i]] || m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();

        //Rendering those windows...
        m_windowsList[windowsIndex[i]]->Render();
    }
}

void ShapePopulationQT::computeRangeDirection_QT()
{
    // Get Attribute in ComboBox
    QString text = this->comboBox_VISU_attribute->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();
    std::ostringstream strs;
    strs.str(""); strs.clear();
    strs << cmap << "_ColorByDirection"<<std::endl;
    std::string cmap_dir = strs.str();
    strs.str(""); strs.clear();
    strs << cmap << "_AbsoluteColorByDirection"<<std::endl;
    std::string cmap_abs = strs.str();
    
    //Get the windows with this attribute
    std::vector<unsigned int > windowsIndex;
    for(unsigned int i = 0 ; i < m_selectedIndex.size() ; i++)
    {
        const char * thisCmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        if(std::string(thisCmap) == std::string(cmap) || std::string(thisCmap) == cmap_dir || std::string(thisCmap) == cmap_abs)
        {
            windowsIndex.push_back(m_selectedIndex[i]);
        }
    }
    
    //Updating this "sameattributewindows" list colormap
    this->computeRangeDirection(cmap);
    
    //Rendering those windows...
    for(unsigned int i = 0 ; i < windowsIndex.size() ; i++)
    {
        m_windowsList[windowsIndex[i]]->Render();
    }
}

void ShapePopulationQT::computeNorm_QT()
{
    // Get Attribute in ComboBox
    QString text = this->comboBox_VISU_attribute->currentText();
    QByteArray arr = text.toLatin1();
    const char *cmap  = arr.data();
    std::ostringstream strs;
    strs.str(""); strs.clear();
    strs << cmap << "_ColorByDirection"<<std::endl;
    std::string cmap_dir = strs.str();
    strs.str(""); strs.clear();
    strs << cmap << "_AbsoluteColorByDirection"<<std::endl;
    std::string cmap_abs = strs.str();
    
    //Get the windows with this attribute
    std::vector<unsigned int > windowsIndex;
    for(unsigned int i = 0 ; i < m_selectedIndex.size() ; i++)
    {
        const char * thisCmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        if(std::string(thisCmap) == std::string(cmap) || std::string(thisCmap) == cmap_dir || std::string(thisCmap) == cmap_abs)
        {
            windowsIndex.push_back(m_selectedIndex[i]);
        }
    }
    
    //Updating this "sameattributewindows" list colormap
    this->computeNorm(cmap);
    
    //Rendering those windows...
    for(unsigned int i = 0 ; i < windowsIndex.size() ; i++)
    {
        m_windowsList[windowsIndex[i]]->Render();
    }
}


void ShapePopulationQT::updateArrowPosition()
{
    m_updateOnPositionChanged = false;
    
    // Update Spinbox ranges
    this->spinBox_VISU_position->setMinimum(m_usedColorBar->range[0]);
    this->spinBox_VISU_position->setMaximum(m_usedColorBar->range[1]);
    double range = fabs(m_usedColorBar->range[1] - m_usedColorBar->range[0]);
    this->spinBox_VISU_position->setSingleStep(range/100);
    
    // Update Spinbox value
    qreal newPos = this->gradientWidget_VISU->getFocusPosition();
    this->slot_gradArrow_moved(newPos);
    
    m_updateOnPositionChanged = true;
}


void ShapePopulationQT::on_spinBox_VISU_min_valueChanged(double min)
{
    if(min > spinBox_VISU_max->value())
    {
        spinBox_VISU_min->setValue(spinBox_VISU_max->value());
        return;
    }
    m_usedColorBar->range[0] = min;
    this->updateColorbar_QT();
    this->updateArrowPosition();
}

void ShapePopulationQT::on_spinBox_VISU_max_valueChanged(double max)
{
    if(max < spinBox_VISU_min->value())
    {
        spinBox_VISU_max->setValue(spinBox_VISU_min->value());
        return;
    }
    m_usedColorBar->range[1] = max;
    this->updateColorbar_QT();
    this->updateArrowPosition();
}


void ShapePopulationQT::on_pushButton_VISU_resetRange_clicked()
{
    m_usedColorBar->range[0] = m_commonRange[0];
    m_usedColorBar->range[1] = m_commonRange[1];
    spinBox_VISU_min->setValue(m_usedColorBar->range[0]);
    spinBox_VISU_max->setValue(m_usedColorBar->range[1]);
    
    this->updateColorbar_QT();
    this->updateArrowPosition();
    
}



/* Range for the color map by direction */
    // X coordinate
void ShapePopulationQT::on_spinBox_VISU_min_AxisX_valueChanged(double newXmin)
{
    if(!m_displayColorMap)
    {
        if(spinBox_VISU_max_AxisX->value() == newXmin)
        {
            return;
        }

        if(newXmin > spinBox_VISU_max_AxisX->value())
        {
            spinBox_VISU_min_AxisX->setValue(spinBox_VISU_max_AxisX->value());
        }

        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            if(m_linkCoordinate)
            {
                double Ymax =  spinBox_VISU_max_AxisY->value();
                double Ymin =  spinBox_VISU_min_AxisY->value();
                double Zmax =  spinBox_VISU_max_AxisZ->value();
                double Zmin =  spinBox_VISU_min_AxisZ->value();

                //            double newXmin = spinBox_VISU_min_AxisX->value();
                double Xmax = spinBox_VISU_max_AxisX->value();

                double newYmin;
                double newZmin;


                double Xmin = m_usedValueDirectionColorMap->min[0];

                newYmin = Ymin + (( newXmin - Xmin ) * ( Ymax - Ymin ))/( Xmax - Xmin );
                newZmin = Zmin + (( newXmin - Xmin ) * ( Zmax - Zmin ))/( Xmax - Xmin );

                m_usedValueDirectionColorMap->min[1] = newYmin;
                spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
                m_usedValueDirectionColorMap->min[2] = newZmin;
                spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
            }
            m_usedValueDirectionColorMap->min[0] = newXmin;
            this->UpdateColorMapByDirection_QT();
        }
    }
}

void ShapePopulationQT::on_spinBox_VISU_max_AxisX_valueChanged(double newXmax)
{
    if(!m_displayColorMap)
    {
        if(newXmax == spinBox_VISU_min_AxisX->value())
        {
            return;
        }

        if(newXmax < spinBox_VISU_min_AxisX->value())
        {
            spinBox_VISU_max_AxisX->setValue(spinBox_VISU_min_AxisX->value());
        }

        if(m_linkCoordinate)
        {
            double Ymax =  spinBox_VISU_max_AxisY->value();
            double Ymin =  spinBox_VISU_min_AxisY->value();
            double Zmax =  spinBox_VISU_max_AxisZ->value();
            double Zmin =  spinBox_VISU_min_AxisZ->value();

            //        double newXmax = spinBox_VISU_max_AxisX->value();
            double Xmin = spinBox_VISU_min_AxisX->value();

            double newYmax;
            double newZmax;

            if(m_displayColorMapByDirection[m_selectedIndex[0]])
            {
                double Xmax = m_usedValueDirectionColorMap->max[0];

                newYmax = Ymax - (( Xmax - newXmax ) * ( Ymax - Ymin ))/( Xmax - Xmin );
                newZmax = Zmax - (( Xmax - newXmax ) * ( Zmax - Zmin ))/( Xmax - Xmin );

                m_usedValueDirectionColorMap->max[1] = newYmax;
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);

                m_usedValueDirectionColorMap->max[2] = newZmax;
                spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);
            }
            else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
            {
                double Xmax = m_usedValueDirectionColorMap->maxAbs[0];

                newYmax = Ymax - (( Xmax - newXmax ) * ( Ymax - Ymin ))/( Xmax - Xmin );
                newZmax = Zmax - (( Xmax - newXmax ) * ( Zmax - Zmin ))/( Xmax - Xmin );


                m_usedValueDirectionColorMap->maxAbs[1] = newYmax;
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);

                m_usedValueDirectionColorMap->maxAbs[2] = newZmax;
                spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
            }

        }

        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->max[0] = newXmax;
            this->UpdateColorMapByDirection_QT();
        }
        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->maxAbs[0] = newXmax;
            this->UpdateColorMapByAbsoluteDirection_QT();
        }
    }
}

void ShapePopulationQT::on_pushButton_VISU_resetRange_AxisX_clicked()
{
    this->computeNorm_QT();
    this->computeRangeDirection_QT();

    m_usedValueDirectionColorMap->norm = m_norm;

    if(m_linkCoordinate)
    {
        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->min[1] = m_commonMin[1];
            m_usedValueDirectionColorMap->max[1] = m_commonMax[1];
            spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
            spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
            m_usedValueDirectionColorMap->min[2] = m_commonMin[2];
            m_usedValueDirectionColorMap->max[2] = m_commonMax[2];
            spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
            spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);
        }
        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->maxAbs[1] = m_commonMaxAbs[1];
            spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);
            m_usedValueDirectionColorMap->maxAbs[2] = m_commonMaxAbs[2];
            spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
        }
    }

    if(m_displayColorMapByDirection[m_selectedIndex[0]])
    {
        m_usedValueDirectionColorMap->min[0] = m_commonMin[0];
        m_usedValueDirectionColorMap->max[0] = m_commonMax[0];
        spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
        spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);
        this->UpdateColorMapByDirection_QT();
    }
    else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
    {
        m_usedValueDirectionColorMap->maxAbs[0] = m_commonMaxAbs[0];
        spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);
        this->UpdateColorMapByAbsoluteDirection_QT();
    }
}

    // Y coordinate
void ShapePopulationQT::on_spinBox_VISU_min_AxisY_valueChanged(double newYmin)
{
    if(!m_displayColorMap)
    {
        if(spinBox_VISU_max_AxisY->value() == newYmin)
        {
            return;
        }

        if(newYmin > spinBox_VISU_max_AxisY->value())
        {
            spinBox_VISU_min_AxisY->setValue(spinBox_VISU_max_AxisY->value());
        }

        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            if(m_linkCoordinate)
            {
                double Xmax =  spinBox_VISU_max_AxisX->value();
                double Xmin =  spinBox_VISU_min_AxisX->value();
                double Zmax =  spinBox_VISU_max_AxisZ->value();
                double Zmin =  spinBox_VISU_min_AxisZ->value();

                //            double newYmin = spinBox_VISU_min_AxisY->value();
                double Ymax = spinBox_VISU_max_AxisY->value();

                double newXmin;
                double newZmin;


                double Ymin = m_usedValueDirectionColorMap->min[1];

                newXmin = Xmin + (( newYmin - Ymin ) * ( Xmax - Xmin ))/( Ymax - Ymin );
                newZmin = Zmin + (( newYmin - Ymin ) * ( Zmax - Zmin ))/( Ymax - Ymin );

                m_usedValueDirectionColorMap->min[0] = newXmin;
                spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
                m_usedValueDirectionColorMap->min[2] = newZmin;
                spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
            }

            m_usedValueDirectionColorMap->min[1] = newYmin;
            this->UpdateColorMapByDirection_QT();
        }
    }
}

void ShapePopulationQT::on_spinBox_VISU_max_AxisY_valueChanged(double newYmax)
{
    if(!m_displayColorMap)
    {
        if(newYmax == spinBox_VISU_min_AxisY->value())
        {
            return;
        }

        if(newYmax < spinBox_VISU_min_AxisY->value())
        {
            spinBox_VISU_max_AxisY->setValue(spinBox_VISU_min_AxisY->value());
        }

        if(m_linkCoordinate)
        {
            double Xmax =  spinBox_VISU_max_AxisX->value();
            double Xmin =  spinBox_VISU_min_AxisX->value();
            double Zmax =  spinBox_VISU_max_AxisZ->value();
            double Zmin =  spinBox_VISU_min_AxisZ->value();

            //        double newYmax = spinBox_VISU_max_AxisY->value();
            double Ymin = spinBox_VISU_min_AxisY->value();

            double newXmax;
            double newZmax;

            if(m_displayColorMapByDirection[m_selectedIndex[0]])
            {
                double Ymax = m_usedValueDirectionColorMap->max[1];

                newXmax = Xmax - (( Ymax - newYmax ) * ( Xmax - Xmin ))/( Ymax - Ymin );
                newZmax = Zmax - (( Ymax - newYmax ) * ( Zmax - Zmin ))/( Ymax - Ymin );

                m_usedValueDirectionColorMap->max[0] = newXmax;
                spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);

                m_usedValueDirectionColorMap->max[2] = newZmax;
                spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);
            }
            else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
            {
                double Ymax = m_usedValueDirectionColorMap->maxAbs[1];

                newXmax = Xmax - (( Ymax - newYmax ) * ( Xmax - Xmin ))/( Ymax - Ymin );
                newZmax = Zmax - (( Ymax - newYmax ) * ( Zmax - Zmin ))/( Ymax - Ymin );

                m_usedValueDirectionColorMap->maxAbs[0] = newXmax;
                spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);

                m_usedValueDirectionColorMap->maxAbs[2] = newZmax;
                spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
            }
        }

        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->max[1] = newYmax;
            this->UpdateColorMapByDirection_QT();
        }
        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->maxAbs[1] = newYmax;
            this->UpdateColorMapByAbsoluteDirection_QT();
        }
    }
}


void ShapePopulationQT::on_pushButton_VISU_resetRange_AxisY_clicked()
{
    this->computeNorm_QT();
    this->computeRangeDirection_QT();

    m_usedValueDirectionColorMap->norm = m_norm;

    if(m_linkCoordinate)
    {
        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->min[0] = m_commonMin[0];
            m_usedValueDirectionColorMap->max[0] = m_commonMax[0];
            spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
            spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);
            m_usedValueDirectionColorMap->min[2] = m_commonMin[2];
            m_usedValueDirectionColorMap->max[2] = m_commonMax[2];
            spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
            spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);
        }
        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->maxAbs[0] = m_commonMaxAbs[0];
            spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);
            m_usedValueDirectionColorMap->maxAbs[2] = m_commonMaxAbs[2];
            spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
        }
    }

    if(m_displayColorMapByDirection[m_selectedIndex[0]])
    {
        m_usedValueDirectionColorMap->min[1] = m_commonMin[1];
        m_usedValueDirectionColorMap->max[1] = m_commonMax[1];
        spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
        spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
        this->UpdateColorMapByDirection_QT();
    }
    else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
    {
        m_usedValueDirectionColorMap->maxAbs[1] = m_commonMaxAbs[1];
        spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);
        this->UpdateColorMapByAbsoluteDirection_QT();
    }
}

    // Z coordinate
void ShapePopulationQT::on_spinBox_VISU_min_AxisZ_valueChanged(double newZmin)
{
    if(!m_displayColorMap)
    {
        if(spinBox_VISU_max_AxisZ->value() == newZmin)
        {
            return;
        }

        if(newZmin > spinBox_VISU_max_AxisZ->value())
        {
            spinBox_VISU_min_AxisZ->setValue(spinBox_VISU_max_AxisZ->value());
        }

        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            if(m_linkCoordinate)
            {
                double Xmax =  spinBox_VISU_max_AxisX->value();
                double Xmin =  spinBox_VISU_min_AxisX->value();
                double Ymax =  spinBox_VISU_max_AxisY->value();
                double Ymin =  spinBox_VISU_min_AxisY->value();

                //            double newZmin = spinBox_VISU_min_AxisZ->value();
                double Zmax = spinBox_VISU_max_AxisZ->value();

                double newXmin;
                double newYmin;

                double Zmin = m_usedValueDirectionColorMap->min[2];

                newXmin = Xmin + (( newZmin - Zmin ) * ( Xmax - Xmin ))/( Zmax - Zmin );
                newYmin = Ymin + (( newZmin - Zmin ) * ( Ymax - Ymin ))/( Zmax - Zmin );

                m_usedValueDirectionColorMap->min[0] = newXmin;
                spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);

                m_usedValueDirectionColorMap->min[1] = newYmin;
                spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
            }

            m_usedValueDirectionColorMap->min[2] = newZmin;
            this->UpdateColorMapByDirection_QT();
        }
    }

}

void ShapePopulationQT::on_spinBox_VISU_max_AxisZ_valueChanged(double newZmax)
{
    if(!m_displayColorMap)
    {
        if(newZmax == spinBox_VISU_min_AxisZ->value())
        {
            return;
        }

        if(newZmax < spinBox_VISU_min_AxisZ->value())
        {
            spinBox_VISU_max_AxisZ->setValue(spinBox_VISU_min_AxisZ->value());
        }

        if(m_linkCoordinate)
        {
            double Xmax =  spinBox_VISU_max_AxisX->value();
            double Xmin =  spinBox_VISU_min_AxisX->value();
            double Ymax =  spinBox_VISU_max_AxisY->value();
            double Ymin =  spinBox_VISU_min_AxisY->value();

            //        double newZmax = spinBox_VISU_max_AxisZ->value();
            double Zmin = spinBox_VISU_min_AxisZ->value();

            double newXmax;
            double newYmax;

            if(m_displayColorMapByDirection[m_selectedIndex[0]])
            {
                double Zmax = m_usedValueDirectionColorMap->max[2];

                newXmax = Xmax - (( Zmax - newZmax ) * ( Xmax - Xmin ))/( Zmax - Zmin );
                newYmax = Ymax - (( Zmax - newZmax ) * ( Ymax - Ymin ))/( Zmax - Zmin );


                m_usedValueDirectionColorMap->max[0] = newXmax;
                spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);

                m_usedValueDirectionColorMap->max[1] = newYmax;
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
            }
            else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
            {
                double Zmax = m_usedValueDirectionColorMap->maxAbs[2];

                newXmax = Xmax - (( Zmax - newZmax ) * ( Xmax - Xmin ))/( Zmax - Zmin );
                newYmax = Ymax - (( Zmax - newZmax ) * ( Ymax - Ymin ))/( Zmax - Zmin );


                m_usedValueDirectionColorMap->maxAbs[0] = newXmax;
                spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);

                m_usedValueDirectionColorMap->maxAbs[1] = newYmax;
                spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);
            }
        }

        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->max[2] = newZmax;
            this->UpdateColorMapByDirection_QT();
        }
        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->maxAbs[2] = newZmax;
            this->UpdateColorMapByAbsoluteDirection_QT();
        }
    }
}


void ShapePopulationQT::on_pushButton_VISU_resetRange_AxisZ_clicked()
{
    this->computeNorm_QT();
    this->computeRangeDirection_QT();

    m_usedValueDirectionColorMap->norm = m_norm;

    if(m_linkCoordinate)
    {
        if(m_displayColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->min[0] = m_commonMin[0];
            m_usedValueDirectionColorMap->max[0] = m_commonMax[0];
            spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
            spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);
            m_usedValueDirectionColorMap->min[1] = m_commonMin[1];
            m_usedValueDirectionColorMap->max[1] = m_commonMax[1];
            spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
            spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
        }
        else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
        {
            m_usedValueDirectionColorMap->maxAbs[0] = m_commonMaxAbs[0];
            spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);
            m_usedValueDirectionColorMap->maxAbs[1] = m_commonMaxAbs[1];
            spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);
        }
    }

    if(m_displayColorMapByDirection[m_selectedIndex[0]])
    {
        m_usedValueDirectionColorMap->min[2] = m_commonMin[2];
        m_usedValueDirectionColorMap->max[2] = m_commonMax[2];
        spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
        spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);
        this->UpdateColorMapByDirection_QT();
    }
    else if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]])
    {
        m_usedValueDirectionColorMap->maxAbs[2] = m_commonMaxAbs[2];
        spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
        this->UpdateColorMapByAbsoluteDirection_QT();
    }
}

void ShapePopulationQT::on_pushButton_VISU_link_coordinate_clicked()
{
    m_linkCoordinate = !m_linkCoordinate;
    if(m_linkCoordinate)pushButton_VISU_link_coordinate->setIcon(QIcon(":/resources/LinkOn.png"));
    else pushButton_VISU_link_coordinate->setIcon(QIcon(":/resources/LinkOff.png"));

}

/* End */

void ShapePopulationQT::on_pushButton_VISU_delete_clicked()
{
    gradientWidget_VISU->deleteFocusArrow();
    
    if(gradientWidget_VISU->getNumberOfArrows() <= 2) pushButton_VISU_delete->setDisabled(true);
    this->updateColorbar_QT();
}

void ShapePopulationQT::on_spinBox_VISU_position_valueChanged(double arg1)
{
    if (m_colorBarList.size() == 0 || m_updateOnPositionChanged == false) return;
    
    //get relative position depending on the range
    double range = fabs(m_usedColorBar->range[1] - m_usedColorBar->range[0]);
    qreal newPos = (arg1-m_usedColorBar->range[0])/range;
    
    //setting the position to the arrow
    if (newPos >= 0.0 && newPos <= 1.0) gradientWidget_VISU->setFocusPosition(newPos);
    else if(newPos < 0.0) gradientWidget_VISU->setFocusPosition(0.0);
    else if(newPos > 1.0) gradientWidget_VISU->setFocusPosition(1.0);
    
    //update
    this->updateColorbar_QT();
}

void ShapePopulationQT::on_pushButton_VISU_add_clicked()
{
    QColor color;
    color = QColorDialog::getColor();
    if (color.isValid())
    {
        gradientWidget_VISU->addArrow(color, 0.7, true);
        this->updateColorbar_QT();
    }
}

void ShapePopulationQT::on_pushButton_VISU_reset_clicked()
{
    gradientWidget_VISU->reset();
    this->updateColorbar_QT();
}

void ShapePopulationQT::on_radioButton_displayColorMapByMagnitude_toggled(bool checked)
{
    if(radioButton_displayColorMapByMagnitude->isChecked())
    {
        stackedWidget_ColorMapByMagnitude->show();
        stackedWidget_ColorMapByDirection->hide();
    }
    if(checkBox_displayAbsoluteColorMapByDirection->isChecked()) checkBox_displayAbsoluteColorMapByDirection->click();

    // display color map by magnitude
    this->displayColorMapByMagnitude(checked);


    this->RenderAll();
}

void ShapePopulationQT::on_radioButton_displayColorMapByDirection_toggled(bool checked)
{
    m_displayColorMap = true;

    if(radioButton_displayColorMapByDirection->isChecked())
    {
        stackedWidget_ColorMapByDirection->show();
        stackedWidget_ColorMapByMagnitude->hide();
    }

    int index = this->comboBox_VISU_attribute->currentIndex();
    m_usedValueDirectionColorMap = m_valueDirectionColorMapList[index];

    spinBox_VISU_min_AxisX->setEnabled(true);
    spinBox_VISU_min_AxisY->setEnabled(true);
    spinBox_VISU_min_AxisZ->setEnabled(true);

    spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
    spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);
    spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
    spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
    spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
    spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);

    // display color map by direction
    if(!m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]]) this->displayColorMapByDirection(checked);

    m_displayColorMap = false;

    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displayAbsoluteColorMapByDirection_toggled(bool checked)
{
    m_displayColorMap = true;

    int index = this->comboBox_VISU_attribute->currentIndex();
    m_usedValueDirectionColorMap = m_valueDirectionColorMapList[index];

    if(!checked) // color map by direction
    {
        // range
        spinBox_VISU_min_AxisX->setEnabled(true);
        spinBox_VISU_min_AxisY->setEnabled(true);
        spinBox_VISU_min_AxisZ->setEnabled(true);

        spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->min[0]);
        spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->max[0]);
        spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->min[1]);
        spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->max[1]);
        spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->min[2]);
        spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->max[2]);
    }
    else if(!m_displayColorMapByMagnitude[m_selectedIndex[0]]) // color map by absolute direction
    {
        // range
        spinBox_VISU_min_AxisX->setDisabled(true);
        spinBox_VISU_min_AxisY->setDisabled(true);
        spinBox_VISU_min_AxisZ->setDisabled(true);

        spinBox_VISU_min_AxisX->setValue(m_usedValueDirectionColorMap->minAbs[0]);
        spinBox_VISU_max_AxisX->setValue(m_usedValueDirectionColorMap->maxAbs[0]);
        spinBox_VISU_min_AxisY->setValue(m_usedValueDirectionColorMap->minAbs[1]);
        spinBox_VISU_max_AxisY->setValue(m_usedValueDirectionColorMap->maxAbs[1]);
        spinBox_VISU_min_AxisZ->setValue(m_usedValueDirectionColorMap->minAbs[2]);
        spinBox_VISU_max_AxisZ->setValue(m_usedValueDirectionColorMap->maxAbs[2]);
    }

    // display color map by absolute direction or by direction
    this->displayAbsoluteColorMapByDirection(checked);
    if(!m_displayAbsoluteColorMapByDirection[m_selectedIndex[0]] && !radioButton_displayColorMapByMagnitude->isChecked())
    {
        this->displayColorMapByDirection(true);
    }

    m_displayColorMap = false;

    this->RenderAll();
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                         COLOR ARROWS                                          * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationQT::slot_gradArrow_moved(qreal newPos)
{
    //Get the absolute position of the arrow
    double range = fabs(m_usedColorBar->range[1] - m_usedColorBar->range[0]);
    double absPos = m_usedColorBar->range[0] + range * newPos;
    
    //set the spinbox value
    spinBox_VISU_position->setValue(absPos);
}

void ShapePopulationQT::slot_gradArrow_selected(qreal newPos)
{
    if(m_numberOfMeshes == 0) return;
    
    if(gradientWidget_VISU->getNumberOfArrows() > 2) pushButton_VISU_delete->setEnabled(true);
    spinBox_VISU_position->setEnabled(true);
    
    this->slot_gradArrow_moved(newPos);
    this->spinBox_VISU_position->setFocus();
    this->spinBox_VISU_position->selectAll();
}

void ShapePopulationQT::slot_gradArrow_doubleClicked()
{
    QColor color = gradientWidget_VISU->getFocusColor();
    color = QColorDialog::getColor(color);
    if (color.isValid())
    {
        gradientWidget_VISU->setFocusColor(color);
    }
    
    this->updateColorbar_QT();
}

void ShapePopulationQT::slot_no_gradArrow_selected()
{
    pushButton_VISU_delete->setDisabled(true);
    spinBox_VISU_position->setDisabled(true);
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                             INFO                                              * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
void ShapePopulationQT::updateInfo_QT()
{
    // Data Array Headers
    QStandardItemModel * model = (QStandardItemModel*)tableView->model();
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Dim")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Range")));
    
	std::ostringstream strs;
    
    if(m_selectedIndex.size() > 1)
    {
		strs.str(""); strs.clear();
		strs << (int)m_selectedIndex.size()
        <<" surfaces selected, select only one"<< std::endl;
        
        //Infos
		this->lineEdit_filename->setText(QString(strs.str().c_str()));
        this->lineEdit_dir->setText(QString(""));
        this->lineEdit_points->setText(QString(""));
        this->lineEdit_cells->setText(QString(""));
        
        // Data Array Values
        for(unsigned int i=0; i < m_commonAttributes.size(); i++)
        {
            //Name
            QStandardItem * dataName = new QStandardItem(QString(m_commonAttributes[i].c_str()));
            model->setItem(i,0,dataName);
            
            //Dimension
            ShapePopulationData * mesh = m_meshList[0];
            int dim = mesh->GetPolyData()->GetPointData()->GetScalars(m_commonAttributes[i].c_str())->GetNumberOfComponents();
            strs.str(""); strs.clear();
            strs <<dim;
            QStandardItem * dimension = new QStandardItem(QString(strs.str().c_str()));
            model->setItem(i,1,dimension);
            
            //Range
            double * range = m_commonRange;
            if(dim == 1)
            {
                range = computeCommonRange(m_commonAttributes[i].c_str(), m_selectedIndex);
            }
            else if(dim == 3)
            {
                strs.str(""); strs.clear();
                strs <<m_commonAttributes[i]<<"_mag"<<std::endl;
                range = computeCommonRange(strs.str().c_str(), m_selectedIndex);
            }
			strs.str(""); strs.clear();
            strs <<"[ "<<range[0]<<" ; "<<range[1]<<" ]";
            QStandardItem * dataRange = new QStandardItem(QString(strs.str().c_str()));
            model->setItem(i,2,dataRange);
        }
    }
    else
    {
        
        //Infos
        unsigned int index = m_selectedIndex[0];
        vtkSmartPointer<vtkPolyData> selectedData = m_meshList[index]->GetPolyData();
        
        this->lineEdit_filename->setText(QString(m_meshList[index]->GetFileName().c_str()));
        this->lineEdit_dir->setText(QString(m_meshList[index]->GetFileDir().c_str()));
		strs.str(""); strs.clear();
        strs << (int)selectedData->GetNumberOfPoints();
		this->lineEdit_points->setText(QString(strs.str().c_str()));
		strs.str(""); strs.clear();
        strs << (int)selectedData->GetNumberOfCells();
        this->lineEdit_cells->setText(QString(strs.str().c_str()));
        
        
        // Data Array Values
        std::vector<std::string> AttributesList = m_meshList[index]->GetAttributeList();
        
        for(unsigned int i=0; i < AttributesList.size(); i++)
        {
            //Name
            QStandardItem * dataName = new QStandardItem(QString(AttributesList[i].c_str()));
            model->setItem(i,0,dataName);
            
            //Dimension
            ShapePopulationData * mesh = m_meshList[index];
            int dim = mesh->GetPolyData()->GetPointData()->GetScalars(AttributesList[i].c_str())->GetNumberOfComponents();
            strs.str(""); strs.clear();
            strs <<dim;
            QStandardItem * dimension = new QStandardItem(QString(strs.str().c_str()));
            model->setItem(i,1,dimension);
            
            //Range
            double * range = m_commonRange;
            if(dim == 1)
            {
                range = computeCommonRange(AttributesList[i].c_str(), m_selectedIndex);
            }
            else if(dim == 3)
            {
                strs.str(""); strs.clear();
                strs <<AttributesList[i]<<"_mag"<<std::endl;
                range = computeCommonRange(strs.str().c_str(), m_selectedIndex);
            }
			strs.str(""); strs.clear();
            strs <<"[ "<<range[0]<<" ; "<<range[1]<<" ]";
            QStandardItem * dataRange = new QStandardItem(QString(strs.str().c_str()));
            model->setItem(i,2,dataRange);
        }
    }
    
    /* Adapt Columns size */
    tableView->resizeColumnToContents(0);
    tableView->resizeColumnToContents(1);
    on_tabWidget_currentChanged(1);
}

void ShapePopulationQT::updateAttribute_QT()
{
    if(m_selectedIndex.size() == 1)  // if new selection (TODO : wrong if unselect one and one left)
    {
        const char * cmap = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        int index = comboBox_VISU_attribute->findText(cmap);
        
        if (index !=comboBox_VISU_attribute->currentIndex() && index != -1) // 1. different attribute (scalar)
        {
            comboBox_VISU_attribute->setCurrentIndex(index);
        }
        else if (index == comboBox_VISU_attribute->currentIndex())  // 2. same attribute but needs to update range
        {
            double * commonRange = computeCommonRange(cmap, m_selectedIndex);
            m_commonRange[0] = commonRange[0];
            m_commonRange[1] = commonRange[1];
        }
        else // (if the scalar attribute cmap is a _mag one, it's vectors)
        {
            const char * cmap_vector = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetVectors()->GetName();
            int index_vector = comboBox_VISU_attribute->findText(cmap_vector);
            
            if (index_vector !=comboBox_VISU_attribute->currentIndex() && index_vector != -1) // 3. different attribute (vector)
            {
                comboBox_VISU_attribute->setCurrentIndex(index_vector);
            }
            else if (index_vector == comboBox_VISU_attribute->currentIndex()) // 4. same attribute but need to update range (magnitude)
            {
                double * commonRange = computeCommonRange(cmap, m_selectedIndex);
                m_commonRange[0] = commonRange[0];
                m_commonRange[1] = commonRange[1];
            }
        }
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                           VECTORS                                             * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //


void ShapePopulationQT::on_slider_meshOpacity_valueChanged(int value)
{
    double val = (double)value/100;
    this->setMeshOpacity(val);
    this->RenderAll();
}

void ShapePopulationQT::on_slider_vectorScale_valueChanged(int value)
{
    double val = (double)value/100;
    this->setVectorScale(val);
    this->RenderAll();
}

void ShapePopulationQT::on_slider_arrowDens_valueChanged(int value)
{
    this->setVectorDensity(value);
    this->RenderAll();
}

void ShapePopulationQT::on_checkBox_displayVectors_toggled(bool checked)
{
    if(checkBox_displayVectors->isChecked())
    {
        if(m_firstDisplayVector)
        {
            radioButton_displayVectorsbyMagnitude->setChecked(true); // by default
            m_firstDisplayVector = false;
        }
        widget_VISU_colorVectors->setEnabled(true);
        widget_VISU_optionVectors->setEnabled(true);

        if(radioButton_displayVectorsbyMagnitude->isChecked()) this->displayVectorsByMagnitude(true);
        else if(radioButton_displayVectorsbyDirection->isChecked()) this->displayVectorsByDirection(true);
        else if(radioButton_displayVectorsByAbsoluteDirection->isChecked()) this->displayVectorsByAbsoluteDirection(true);

    }
    else
    {
        this->displayVectorsByMagnitude(false);
        this->displayVectorsByDirection(false);
        this->displayVectorsByAbsoluteDirection(false);

        widget_VISU_colorVectors->setDisabled(true);
        widget_VISU_optionVectors->setDisabled(true);
    }
    this->displayVectors(checked);

    this->RenderAll();
}

void ShapePopulationQT::on_radioButton_displayVectorsbyMagnitude_toggled(bool checked)
{
    this->displayVectorsByMagnitude(checked);

    this->RenderAll();
}

void ShapePopulationQT::on_radioButton_displayVectorsbyDirection_toggled(bool checked)
{
    this->displayVectorsByDirection(checked);

    this->RenderAll();
}

void ShapePopulationQT::on_radioButton_displayVectorsByAbsoluteDirection_toggled(bool checked)
{
    this->displayVectorsByAbsoluteDirection(checked);

    this->RenderAll();
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            EXPORT                                             * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
#ifndef SPV_EXTENSION
void ShapePopulationQT::exportToPS()
{
    if (this->getExportDirectory() == 0) return;
    this->exportTo(0);
}
void ShapePopulationQT::exportToEPS()
{
    if (this->getExportDirectory() == 0) return;
    this->exportTo(1);
}
void ShapePopulationQT::exportToPDF()
{
    if (this->getExportDirectory() == 0) return;
    this->exportTo(2);
}
void ShapePopulationQT::exportToTEX()
{
    if (this->getExportDirectory() == 0) return;
    this->exportTo(3);
}
void ShapePopulationQT::exportToSVG()
{
    if (this->getExportDirectory() == 0) return;
    this->exportTo(4);
}

int ShapePopulationQT::getExportDirectory()
{
    QFileDialog dirWindow;
    QString dir = dirWindow.getExistingDirectory(this,tr("Save to Directory"),m_exportDirectory);
    if(dir.isEmpty()) return 0;
    
    m_exportDirectory= dir;
    return 1;
}

void ShapePopulationQT::exportTo(int fileFormat)
{
    vtkGL2PSExporter * exporter = vtkGL2PSExporter::New();
    exporter->SetFileFormat(fileFormat); //see vtkGL2PSExporter::OutputFormat
    exporter->CompressOff();
    
    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        QFileInfo meshfile(mesh->GetFileName().c_str());
        QString meshName = meshfile.baseName();
        QString meshAttribute(mesh->GetPolyData()->GetPointData()->GetScalars()->GetName());
        QString filePrefix = m_exportDirectory + "/" + meshName + "_" + meshAttribute;
        
        exporter->SetInput(m_windowsList[m_selectedIndex[i]]);
        std::string prefixString = filePrefix.toStdString() ;
        
        prefixString = prefixString.erase( prefixString.find_last_not_of( " \n\r\t") + 1 ) ;
        exporter->SetFilePrefix(prefixString.c_str());
        exporter->Write();
    }
    
    exporter->Delete();
}
#endif

void ShapePopulationQT::showNoExportWindow()
{
    std::ostringstream strs;
    strs << "GL2PS is not available in VTK's version of 3D Slicer." << std::endl << std::endl
    << "To export your windows in high-resolution files, you can use the full "
    << "version of ShapePopulationViewer, available on its NITRC webpage"<< std::endl
    << "http://www.nitrc.org/projects/shapepopviewer"<< std::endl;
    
    QMessageBox::about(this,"Not available in Slicer",QString(strs.str().c_str()));
}

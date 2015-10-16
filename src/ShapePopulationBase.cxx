#include "ShapePopulationBase.h"


namespace spv_math
{
#ifdef WIN32
	long round(double a) // round not defined on windows // no positive/negative statement because always called for exp()>0 (see l.156)
	{
        return ((a-(long)a)>=0.5)?(long)a+1:(long)a;
	}
#endif
    
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
}


ShapePopulationBase::ShapePopulationBase()
{
    m_headcam = vtkSmartPointer<vtkCamera>::New();
    m_headcam->ParallelProjectionOn();
    
    m_unselectedColor[0] = 0.0;
    m_unselectedColor[1] = 0.0;
    m_unselectedColor[2] = 0.0;
    m_selectedColor[0] = 0.1;
    m_selectedColor[1] = 0.0;
    m_selectedColor[2] = 0.3;
    m_labelColor[0] = 1.0;
    m_labelColor[1] = 1.0;
    m_labelColor[2] = 1.0;
    m_norm = 0.0;
    
    m_renderAllSelection = false; //changed
    m_displayColorbar = true;
    m_displayAttribute = true;
    m_displayMeshName = true;
    m_displaySphere = true;
    m_displayAxis = true;
    m_displayTitles = true;
    m_noUpdateVectorsByDirection = false;
    m_createAxis.push_back(false);
    m_createSphere.push_back(false);
    m_createTitleSphere.push_back(false);
    m_createTitleAxis.push_back(false);

    
}

void ShapePopulationBase::setBackgroundSelectedColor(double a_selectedColor[])
{
    m_selectedColor[0] = a_selectedColor[0];
    m_selectedColor[1] = a_selectedColor[1];
    m_selectedColor[2] = a_selectedColor[2];
    
    for(unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetBackground(m_selectedColor);
        m_windowsList[m_selectedIndex[i]]->Render();
    }
}

void ShapePopulationBase::setBackgroundUnselectedColor(double a_unselectedColor[])
{
    m_unselectedColor[0] = a_unselectedColor[0];
    m_unselectedColor[1] = a_unselectedColor[1];
    m_unselectedColor[2] = a_unselectedColor[2];
    
    for(unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        m_windowsList[i]->GetRenderers()->GetFirstRenderer()->SetBackground(m_unselectedColor);
    }
    
    for(unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetBackground(m_selectedColor);
    }
    
    this->RenderAll();
}


void ShapePopulationBase::setLabelColor(double a_labelColor[])
{
    m_labelColor[0] = a_labelColor[0];
    m_labelColor[1] = a_labelColor[1];
    m_labelColor[2] = a_labelColor[2];

    for (unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        
        //CornerAnnotation Update
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        vtkSmartPointer<vtkTextProperty> cornerProperty = cornerAnnotation->GetTextProperty();
        cornerProperty->SetColor(m_labelColor);
        
        //CornerAnnotation Update
        viewPropObject = propCollection->GetItemAsObject(3);
        cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        cornerProperty = cornerAnnotation->GetTextProperty();
        cornerProperty->SetColor(m_labelColor);
        
        //ScalarBar Update
        viewPropObject = propCollection->GetItemAsObject(4);
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)viewPropObject;
        vtkSmartPointer<vtkTextProperty> labelProperty = scalarBar->GetLabelTextProperty();
        labelProperty->SetColor(m_labelColor);
        

        // Title of the axis
        if(!m_widgetTitleAxis.empty()) deleteTitleAxisWidget(i);
        if(m_displayVectors[i])
        {
            if((m_displayAbsoluteColorMapByDirection[i] && !m_displayVectorsByAbsoluteDirection[i]) || (!m_displayAbsoluteColorMapByDirection[i] && m_displayVectorsByAbsoluteDirection[i]))
            {
                this->creationTitleAxisWidget(i);
            }
        }

        // Title of the sphere
        if(!m_widgetTitleSphere.empty()) deleteTitleSphereWidget(i);
        if(m_displayVectors[i])
        {
            if((m_displayColorMapByDirection[i] && !m_displayVectorsByDirection[i]) || (!m_displayColorMapByDirection[i] && m_displayVectorsByDirection[i]))
            {
                this->creationTitleSphereWidget(i);
            }
        }

        m_windowsList[i]->Render();
    }
}

void ShapePopulationBase::CreateNewWindow(std::string a_filePath)
{
    //DATA
    ShapePopulationData * Mesh = new ShapePopulationData;
    Mesh->ReadMesh(a_filePath);
    m_meshList.push_back(Mesh);
    
    //MAPPER
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if (VTK_MAJOR_VERSION < 6)
    mapper->SetInputConnection(Mesh->GetPolyData()->GetProducerPort());
#else
    mapper->SetInputData(Mesh->GetPolyData());
#endif
    
    
    //ACTOR
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    
    /* VECTORS */
    
    //Arrow
    
    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
#if (VTK_MAJOR_VERSION < 6)
    glyph->SetInputConnection(Mesh->GetPolyData()->GetProducerPort());
#else
    vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
    glyph->SetSourceConnection(arrow->GetOutputPort());
    glyph->SetInputData(Mesh->GetPolyData());
#endif
    glyph->ScalingOn();
    glyph->OrientOn();
    glyph->ClampingOff();
    glyph->SetColorModeToColorByVector();
    glyph->SetScaleModeToScaleByVector();
    glyph->SetVectorModeToUseVector();
    glyph->Update();
    m_glyphList.push_back(glyph);
    
    //Mapper & Actor
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if (VTK_MAJOR_VERSION < 6)
    glyphMapper->SetInputConnection(glyph->GetOutputPort());
#else
    glyphMapper->SetInputData(glyph->GetOutput());
#endif
    vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
    glyphActor->SetMapper(glyphMapper);
    
    /* END VECTORS */
    
    
    //RENDERER
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->AddActor(glyphActor);
    renderer->SetActiveCamera(m_headcam); //set the active camera for this renderer to main camera
    renderer->ResetCamera();
    //renderer->SetUseDepthPeeling(true);/*test opacity*/
    
    //WINDOW
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    //renderWindow->SetAlphaBitPlanes(true);/*test opacity*/
    //renderWindow->SetMultiSamples(0);/*test opacity*/
    m_windowsList.push_back(renderWindow);

    //INTERACTOR
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    //ANNOTATIONS (file name)
    vtkSmartPointer<vtkCornerAnnotation> fileName = vtkSmartPointer<vtkCornerAnnotation>::New();
    fileName->SetLinearFontScaleFactor( 2 );
    fileName->SetNonlinearFontScaleFactor( 1 );
    fileName->SetMaximumFontSize( 15 );
    fileName->SetText(2,Mesh->GetFileName().c_str());
    fileName->GetTextProperty()->SetColor(m_labelColor);
    renderer->AddViewProp(fileName);
    
    
    //ANNOTATIONS (attribute name)
    vtkSmartPointer<vtkCornerAnnotation> attributeName = vtkSmartPointer<vtkCornerAnnotation>::New();
    attributeName = vtkSmartPointer<vtkCornerAnnotation>::New();
    attributeName->SetLinearFontScaleFactor(2);
    attributeName->SetNonlinearFontScaleFactor(1);
    attributeName->SetMaximumFontSize(15);
    attributeName->SetText(0," ");
    attributeName->GetTextProperty()->SetColor(m_labelColor);
    renderer->AddViewProp(attributeName);
    
    // SCALAR BAR
    vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetTitle("Title");
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetMaximumWidthInPixels(60);
    
    vtkSmartPointer<vtkTextProperty> LabelProperty = vtkSmartPointer<vtkTextProperty>::New();
    LabelProperty->SetFontSize(12);
    LabelProperty->SetColor(m_labelColor);
    
    scalarBar->SetLabelTextProperty(LabelProperty);
    scalarBar->SetTitleTextProperty(LabelProperty);
    
    renderer->AddActor2D(scalarBar);
    
    //DISPLAY
    if (m_displayMeshName == false) fileName->SetVisibility(0);
    if (m_displayAttribute == false) attributeName->SetVisibility(0);
    if (m_displayColorbar == false) scalarBar->SetVisibility(0);
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                          SELECTION                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

unsigned int ShapePopulationBase::getSelectedIndex(vtkSmartPointer<vtkRenderWindow> a_selectedWindow)
{
    unsigned int j=0;
    for( j=0 ; j < m_windowsList.size() ; j++)
    {
        if (a_selectedWindow == m_windowsList[j]) break;
    }
    return j;
}

void ShapePopulationBase::ClickEvent(vtkObject* a_selectedObject, unsigned long, void*)
{
    /* IN GUI VERSION, DO
     ** return if everything already selected
     ** {...this_function...}
     ** an "else" for colormap, to update combobox attribute displayed
     ** Center display
     ** allow GUI actions
     ** check Select All if needed
     */
    
    //Get the interactor used
    vtkSmartPointer<vtkRenderWindowInteractor> selectedInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    selectedInteractor = (vtkRenderWindowInteractor*)a_selectedObject;
    vtkSmartPointer<vtkRenderWindow> selectedWindow = selectedInteractor->GetRenderWindow();
    unsigned int index = getSelectedIndex(selectedWindow);
    
    //if the renderwindow already is in the renderselectedWindows
    if( (std::find(m_selectedIndex.begin(), m_selectedIndex.end(), index)) != (m_selectedIndex.end()) )
    {
        // UNSELECTING
        if(selectedInteractor->GetControlKey() == 1)
        {
            selectedWindow->GetRenderers()->GetFirstRenderer()->SetBackground(m_unselectedColor);
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
            camera->DeepCopy(m_headcam);
            selectedWindow->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
            selectedWindow->Render();
            m_selectedIndex.erase((std::find(m_selectedIndex.begin(), m_selectedIndex.end(), index)));
        }
        return;
    }
    
    // NEW SELECTION (Ctrl not pushed)
    if(selectedInteractor->GetControlKey() == 0)
    {
        for (unsigned int i = 0; i < m_selectedIndex.size();i++)                              //reset backgrounds and cameras
        {
            m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetBackground(m_unselectedColor);
            vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
            camera->DeepCopy(m_headcam);
            m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
            m_windowsList[m_selectedIndex[i]]->Render();
        }
        m_selectedIndex.clear();                                                             // empty the selectedWindows list
    }
    
    // SELECTING
    selectedWindow->GetRenderers()->GetFirstRenderer()->SetBackground(m_selectedColor);                 //Background color to grey
    if(m_selectedIndex.empty())                                                              //Copy last camera if new selection
    {
        m_headcam->DeepCopy(selectedWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera());
    }
    selectedWindow->GetRenderers()->GetFirstRenderer()->SetActiveCamera(m_headcam);                 //Set renderWindow to headcam
    m_selectedIndex.push_back(index);                                               //Add to the selectedWindows List
    
    // IF MULTIPLE SELECTION
    if(m_selectedIndex.size() > 1)
    {
        // Update colormap to the first selected window position
        const char * cmap = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        const char * cmap_newSelected = m_meshList[index]->GetPolyData()->GetPointData()->GetScalars()->GetName();

        if(strcmp(cmap,cmap_newSelected))
        {
            m_noUpdateVectorsByDirection = true;
        }
        this->UpdateAttribute(cmap, m_selectedIndex);
//        int dim = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars(cmap)->GetNumberOfComponents();
        /*if(dim == 1)*/ this->UpdateColorMap(m_selectedIndex);
    }
    
    
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
}

void ShapePopulationBase::SelectAll()
{
    /* IN GUI VERSION, DO
     ** {...this_function...}
     ** enable GUI actions
     ** check Select All if
     ** display the infos you need
     */

    m_selectedIndex.clear();
    for(unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        m_selectedIndex.push_back(i);
        m_windowsList[i]->GetRenderers()->GetFirstRenderer()->SetActiveCamera(m_headcam); //connect to headcam for synchro
        m_windowsList[i]->GetRenderers()->GetFirstRenderer()->SetBackground(m_selectedColor);
        m_windowsList[i]->Render();
    }
}

void ShapePopulationBase::UnselectAll()
{
    /* IN GUI VERSION, DO
     ** {...this_function...}
     ** disable GUI actions
     ** uncheck Select All if needed
     */
    
    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        //Create an independant camera, copy of headcam
        vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
        camera->DeepCopy(m_headcam);
        
        m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
        m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetBackground(m_unselectedColor);
        m_windowsList[m_selectedIndex[i]]->Render();
    }
    m_selectedIndex.clear();
}

void ShapePopulationBase::KeyPressEventVTK(vtkObject* a_selectedObject, unsigned long , void* )
{
    vtkSmartPointer<vtkRenderWindowInteractor> selectedInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    selectedInteractor = (vtkRenderWindowInteractor*)a_selectedObject;
    
    //UNSELECTING - GetKeySym = Escape, GetKeyCode() = 27
    if(selectedInteractor->GetKeyCode() == (char)27 )
    {
        UnselectAll();
    }
    //SELECT ALL - GetControlKey != 0 & GetKeySym()="a"
    else if(selectedInteractor->GetControlKey() != 0 && strcmp(selectedInteractor->GetKeySym(),"a") == 0)
    {
        SelectAll();
    }
}

void ShapePopulationBase::CameraChangedEventVTK(vtkObject*, unsigned long, void*)
{
    this->UpdateCameraConfig();
}

void ShapePopulationBase::StartEventVTK(vtkObject*, unsigned long, void*)
{
    m_renderAllSelection = true;
}

void ShapePopulationBase::EndEventVTK(vtkObject*, unsigned long, void*)
{
    m_renderAllSelection = false;
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                           RENDERING                                           * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::RenderAll()
{
    for (unsigned int i = 0; i < m_windowsList.size();i++)
    {
        m_windowsList[i]->Render();
    }
}
void ShapePopulationBase::RenderSelection()
{
    if(m_selectedIndex.size()==0 || m_renderAllSelection == false) return;
    
    int test_realtime = m_windowsList[m_selectedIndex[0]]->HasObserver(vtkCommand::RenderEvent);
    int test_delayed = m_windowsList[m_selectedIndex[0]]->HasObserver(vtkCommand::ModifiedEvent);
    
    for (unsigned int i = 0; i < m_selectedIndex.size();i++) //disable the renderWindows to callback RenderSelection again
    {
        m_windowsList[m_selectedIndex[i]]->RemoveAllObservers();
    }
    
    for (unsigned int i = 0; i < m_selectedIndex.size();i++) //render all windows selected (one of them will be the event window)
    {
        m_windowsList[m_selectedIndex[i]]->Render();
    }
    
    for (unsigned int i = 0; i < m_selectedIndex.size();i++) //attribuate the observers back to the windows the way it used to be
    {
        if(test_realtime)
        {
            m_windowsList[m_selectedIndex[i]]->AddObserver(vtkCommand::RenderEvent, this, &ShapePopulationBase::RenderSelection);
        }
        else if (test_delayed)
        {
            m_windowsList[m_selectedIndex[i]]->AddObserver(vtkCommand::ModifiedEvent, this, &ShapePopulationBase::RenderSelection);
        }
    }
    
    //this->sendCameraConfig();
}

void ShapePopulationBase::RealTimeRenderSynchro(bool realtime)
{
    if(realtime)
    {
        for (unsigned int i = 0; i < m_windowsList.size(); i++)
        {
            //syncronize when rendering
            m_windowsList[i]->RemoveAllObservers();
            m_windowsList[i]->AddObserver(vtkCommand::RenderEvent, this, &ShapePopulationBase::RenderSelection);
        }
    }
    if(!realtime)
    {
        for (unsigned int i = 0; i < m_windowsList.size(); i++)
        {
            //syncronize when render modified
            m_windowsList[i]->RemoveAllObservers();
            m_windowsList[i]->AddObserver(vtkCommand::ModifiedEvent, this, &ShapePopulationBase::RenderSelection);
        }
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                           COLORMAP                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::computeCommonAttributes()
{
    m_commonAttributes.clear();
    
    for (unsigned int i = 0; i < m_meshList.size(); i++)
    {
        int numScalars = m_meshList.at(i)->GetAttributeList().size();
        if(i==0)
        {
            for (int j = 0; j < numScalars; j++)
            {
                std::string scalarName = m_meshList.at(i)->GetAttributeList().at(j);
                m_commonAttributes.push_back(scalarName);
            }
        }
        else
        {
            std::vector<std::string> Attributes2;
            for (int j = 0; j < numScalars; j++)
            {
                std::string scalarName = m_meshList.at(i)->GetAttributeList().at(j);
                Attributes2.push_back(scalarName);
            }
            
            std::vector<std::string> commonAttributes;
            std::set_intersection(m_commonAttributes.begin(),
                                  m_commonAttributes.end(),
                                  Attributes2.begin(),
                                  Attributes2.end(),
                                  std::back_inserter(commonAttributes));
            m_commonAttributes = commonAttributes;
        }
    }
}

double * ShapePopulationBase::computeCommonRange(const char * a_cmap, std::vector< unsigned int > a_windowIndex)
{
    double * commonRange = NULL; //to avoid warning for not being initialized
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        double * newRange = m_meshList[a_windowIndex[i]]->GetPolyData()->GetPointData()->GetScalars(a_cmap)->GetRange();
        
        if(i==0) commonRange = newRange;
        else
        {
            if(newRange[0] < commonRange[0]) commonRange[0] = newRange[0];
            if(newRange[1] > commonRange[1]) commonRange[1] = newRange[1];
        }
    }
    
    return commonRange;
}

void ShapePopulationBase::computeRangeDirection(const char * a_cmap)
{
    double min[3];
    double max[3];
    double minAbs[3];
    double maxAbs[3];
    
    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        //        const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
        std::string cmap = std::string(a_cmap);
        std::string key1 ("_ColorByDirection");
        size_t found = cmap.rfind(key1);
        cmap = cmap.substr(0,found);
        std::string key2 ("_AbsoluteColorByDirection");
        found = cmap.rfind(key2);
        cmap = cmap.substr(0,found);
        std::string key3 ("_mag");
        found = cmap.rfind(key3);
        cmap = cmap.substr(0,found);
        
        int numPts = mesh->GetPolyData()->GetPoints()->GetNumberOfPoints();
        vtkDataArray *vector;
        vector = mesh->GetPolyData()->GetPointData()->GetScalars(cmap.c_str());
        
        // Compute the minimum and maximum
        for(int j = 0; j < 3; j++)
        {
            std::vector<double> vectXYZ;
            std::vector<double> vectXYZAbs;
            for( int l = 0; l < numPts; ++l )
            {
                vectXYZ.push_back(vector->GetComponent(l,j)/m_norm);
                vectXYZAbs.push_back((fabs(vector->GetComponent(l,j)))/m_norm);
            }
            std::sort (vectXYZ.begin(), vectXYZ.end()); // using default comparison (operator <)
            min[j] = vectXYZ[0];
            max[j] = vectXYZ[numPts-1];
            std::sort (vectXYZAbs.begin(), vectXYZAbs.end()); // using default comparison (operator <)
            minAbs[j] = 0.0;
            maxAbs[j] = vectXYZAbs[numPts-1];
        }
        
        // Compute the largest range
        if(i == 0)
        {
            for(int cc = 0; cc < 3 ; cc++)
            {
                m_commonMin[cc] = min[cc];
                m_commonMax[cc] = max[cc];
                m_commonMinAbs[cc] = minAbs[cc];
                m_commonMaxAbs[cc] = maxAbs[cc];
            }
        }
        else
        {
            for(int cc = 0; cc < 3 ; cc++)
            {
                if(m_commonMin[cc] > min[cc]) m_commonMin[cc] = min[cc];
                if(m_commonMax[cc] < max[cc]) m_commonMax[cc] = max[cc];
                if(m_commonMinAbs[cc] > minAbs[cc]) m_commonMinAbs[cc] = minAbs[cc];
                if(m_commonMaxAbs[cc] < maxAbs[cc]) m_commonMaxAbs[cc] = maxAbs[cc];
            }
        }
    }
}

void ShapePopulationBase::computeNorm(const char * a_cmap)
{
    double norm;
    
    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        std::string cmap = std::string(a_cmap);
        std::string key1 ("_ColorByDirection");
        size_t found = cmap.rfind(key1);
        cmap = cmap.substr(0,found);
        std::string key2 ("_AbsoluteColorByDirection");
        found = cmap.rfind(key2);
        cmap = cmap.substr(0,found);
        std::string key3 ("_mag");
        found = cmap.rfind(key3);
        cmap = cmap.substr(0,found);
        
        norm = mesh->GetPolyData()->GetPointData()->GetScalars(cmap.c_str())->GetMaxNorm();
        
        // Compute the bigger norm
        if(i == 0) m_norm = norm;
        else
        {
            if(m_norm < norm) m_norm = norm;
        }
    }
}


void ShapePopulationBase::UpdateColorMapByDirection(const char * cmap,int index)
{
    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        
        int numPts = mesh->GetPolyData()->GetPoints()->GetNumberOfPoints();
        std::string new_cmap = std::string(cmap);
        std::string key1 ("_ColorByDirection");
        size_t found = new_cmap.rfind(key1);
        new_cmap = new_cmap.substr(0,found);
        std::string key2 ("_AbsoluteColorByDirection");
        found = new_cmap.rfind(key2);
        new_cmap = new_cmap.substr(0,found);
        std::string key3 ("_mag");
        found = new_cmap.rfind(key3);
        new_cmap = new_cmap.substr(0,found);
        
        std::ostringstream strs;
        strs.str("");
        strs.clear();
        strs << new_cmap << "_ColorByDirection" << std::endl;
        
        vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
        scalars->SetNumberOfComponents(3);
        scalars->SetNumberOfTuples(numPts);
        scalars->SetName(strs.str().c_str());
        
        double RGB[3];
        vtkDataArray *vector;
        vector = mesh->GetPolyData()->GetPointData()->GetScalars(cmap);
        
        m_usedValueDirectionColorMap = m_valueDirectionColorMapList[index];
        
        double norm = m_usedValueDirectionColorMap->norm;
        double max[3];
        double min[3];
        for(int cc = 0; cc < 3; cc++)
        {
            min[cc] = m_usedValueDirectionColorMap->min[cc];
            max[cc] = m_usedValueDirectionColorMap->max[cc];
        }
        
        // RGB scalar corresponding
        for( int l = 0; l < numPts; ++l )
        {
            for(int k = 0; k < 3; k++)
            {
                if((vector->GetComponent(l,k)/norm) >= min[k] && (vector->GetComponent(l,k)/norm) <= max[k])
                {
                    if(max[k] != min[k]) RGB[k] = ((255/(max[k]-min[k]))*(vector->GetComponent(l,k)/norm)-(255*min[k])/(max[k]-min[k]));
                    else if (max[k] != 0) RGB[k] = 255;
                    else if (max[k] == 0) RGB[k] = 0;
                }
                else if((vector->GetComponent(l,k)/norm) < min[k]) RGB[k] = 0;
                else if((vector->GetComponent(l,k)/norm) > max[k]) RGB[k] = 255;
            }
            scalars->InsertTuple(l,RGB);
        }
        mesh->GetPolyData()->GetPointData()->AddArray(scalars);
    }
    
}

void ShapePopulationBase::UpdateColorMapByAbsoluteDirection(const char * cmap, int index)
{
    for (unsigned int i = 0; i < m_selectedIndex.size(); i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        
        int numPts = mesh->GetPolyData()->GetPoints()->GetNumberOfPoints();
        std::string new_cmap = std::string(cmap);
        std::string key1 ("_ColorByDirection");
        size_t found = new_cmap.rfind(key1);
        new_cmap = new_cmap.substr(0,found);
        std::string key2 ("_AbsoluteColorByDirection");
        found = new_cmap.rfind(key2);
        new_cmap = new_cmap.substr(0,found);
        std::string key3 ("_mag");
        found = new_cmap.rfind(key3);
        new_cmap = new_cmap.substr(0,found);
        
        std::ostringstream strs;
        strs.str("");
        strs.clear();
        strs << new_cmap << "_AbsoluteColorByDirection" << std::endl;
        
        vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
        scalars->SetNumberOfComponents(3);
        scalars->SetNumberOfTuples(numPts);
        scalars->SetName(strs.str().c_str());
        
        double RGB[3];
        vtkDataArray *vector;
        vector = mesh->GetPolyData()->GetPointData()->GetScalars(cmap);
        
        m_usedValueDirectionColorMap = m_valueDirectionColorMapList[index];
        
        double norm = m_usedValueDirectionColorMap->norm;
        double max[3];
        double min[3];
        for(int cc = 0; cc < 3; cc++)
        {
            min[cc] = m_usedValueDirectionColorMap->minAbs[cc];
            max[cc] = m_usedValueDirectionColorMap->maxAbs[cc];
        }
        
        // RGB scalar corresponding
        for( int l = 0; l < numPts; ++l )
        {
            for(int k = 0; k < 3; k++)
            {
                if(fabs(vector->GetComponent(l,k))/norm >= min[k] && fabs(vector->GetComponent(l,k))/norm <= max[k])
                {
                    if(max[k] != min[k]) RGB[k] = ((255/(max[k]-min[k]))*(fabs(vector->GetComponent(l,k))/norm)-(255*min[k])/(max[k]-min[k]));
                    else if (max[k] != 0) RGB[k] = 255;
                    else if (max[k] == 0) RGB[k] = 0;
                }
                else if((fabs(vector->GetComponent(l,k))/norm) < min[k]) RGB[k] = 0;
                else if((fabs(vector->GetComponent(l,k))/norm) > max[k]) RGB[k] = 255;
            }
            scalars->InsertTuple(l,RGB);
        }
        mesh->GetPolyData()->GetPointData()->AddArray(scalars);
    }
}


void ShapePopulationBase::UpdateAttribute(const char * a_cmap, std::vector< unsigned int > a_windowIndex)
{

    /* FIND DIMENSION OF ATTRIBUTE */
    int dim = m_meshList[a_windowIndex[0]]->GetPolyData()->GetPointData()->GetScalars(a_cmap)->GetNumberOfComponents();
    
    //test if _mag => in that case, we will take the cmap without _mag for the vectors
    std::string cmap = std::string(a_cmap);
    size_t found = cmap.rfind("_mag");
    std::string new_cmap = cmap.substr(0,found);
    found = cmap.rfind("_ColorByDirection");
    new_cmap = new_cmap.substr(0,found);
    found = cmap.rfind("_AbsoluteColorByDirection");
    new_cmap = new_cmap.substr(0,found);
    if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()))
    {
        dim = 3;
        a_cmap = new_cmap.c_str();
    }
    
    /* UPDATE ATTRIBUTE NAME (cornerAnnotation) */
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        
        //CornerAnnotation Update
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        cornerAnnotation->ClearAllTexts();
        cornerAnnotation->SetText(0,a_cmap);
    }
    
    /* ATTRIBUTE : SCALARS */
    if(dim == 1)
    {
        for (unsigned int i = 0; i < a_windowIndex.size(); i++)
        {
            ShapePopulationData * mesh = m_meshList[a_windowIndex[i]];
            vtkRenderWindow * window = m_windowsList[a_windowIndex[i]];
            vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
            
            // Set Active Scalars
            mesh->GetPolyData()->GetPointData()->SetActiveScalars(a_cmap);
            
            // Glyph visibility
            glyphActor->SetVisibility(0);
        }

        // Compute the largest range
        //        double * commonRange = computeCommonRange(a_cmap, a_windowIndex);
        double * commonRange = computeCommonRange(a_cmap, m_selectedIndex);
        m_commonRange[0] = commonRange[0];
        m_commonRange[1] = commonRange[1];
    }
    
    /* ATTRIBUTE : VECTORS */
    else if(dim == 3)
    {
        std::ostringstream strs_mag;
        std::ostringstream strs_dir;
        std::ostringstream strs_abs;
        
        for (unsigned int i = 0; i < a_windowIndex.size(); i++)
        {
            strs_mag.str(""); strs_mag.clear();
            strs_mag << a_cmap << "_mag" << std::endl;
            strs_dir.str(""); strs_dir.clear();
            strs_dir << a_cmap << "_ColorByDirection" << std::endl;
            strs_abs.str(""); strs_abs.clear();
            strs_abs << a_cmap << "_AbsoluteColorByDirection" << std::endl;
            
            ShapePopulationData * mesh = m_meshList[a_windowIndex[i]];
            vtkRenderWindow * window = m_windowsList[a_windowIndex[i]];
            vtkSmartPointer<vtkActor> glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
            
            // Set Active Vectors
            mesh->GetPolyData()->GetPointData()->SetActiveVectors(a_cmap);
            
            // Set Active scalars
            // display colormap by magnitude
            if(m_displayColorMapByMagnitude[a_windowIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_mag.str().c_str());
            
            // display colormap by direction
            else if(m_displayColorMapByDirection[a_windowIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_dir.str().c_str());
            
            // display colormap by absolute direction
            else if(m_displayAbsoluteColorMapByDirection[a_windowIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_abs.str().c_str());
                        
            // Update Glyph
            vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
            vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[a_windowIndex[i]];
            glyph->SetSourceConnection(arrow->GetOutputPort());
            glyph->Update();
            
            // Vectors
            if(!m_noUpdateVectorsByDirection)
            {
                if(m_displayVectorsByDirection[a_windowIndex[i]] || m_displayVectorsByAbsoluteDirection[a_windowIndex[i]])
                {
                    this->UpdateVectorsByDirection();
                }
            }
            
            // Glyph visibility
            if (m_displayVectors[a_windowIndex[i]]) glyphActor->SetVisibility(1);
            else glyphActor->SetVisibility(0);
        }
        
        // Compute the largest range
        //        double * commonRange = computeCommonRange(strs_mag.str().c_str(), a_windowIndex);
        double * commonRange = computeCommonRange(strs_mag.str().c_str(), m_selectedIndex);
        m_commonRange[0] = commonRange[0];
        m_commonRange[1] = commonRange[1];
        
    }
    
    /* DISPLAY OF SCALAR BAR */
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)viewPropObject;
        if(m_displayColorMapByMagnitude[a_windowIndex[i]] || m_displayVectorsByMagnitude[a_windowIndex[i]])
        {
            if(m_displayColorbar) scalarBar->SetVisibility(1);
        }
        else
        {
            scalarBar->SetVisibility(0);
        }
    }    
}

void ShapePopulationBase::displayColorMapByMagnitude(bool display)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display) m_displayColorMapByMagnitude[m_selectedIndex[i]] = true ;
        else m_displayColorMapByMagnitude[m_selectedIndex[i]] = false;

        if(display)
        {
            // display of the color map by magnitude
            ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
            const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
            std::string cmap = std::string(a_cmap);
            std::string key1 ("_ColorByDirection");
            size_t found = cmap.rfind(key1);
            if (found!=std::string::npos)
                cmap.replace (found,key1.length(),"_mag");
            std::string key2 ("_AbsoluteColorByDirection");
            found = cmap.rfind(key2);
            if (found!=std::string::npos)
                cmap.replace (found,key2.length(),"_mag");
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << cmap;

            // Set Active Scalars
            mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs.str().c_str());

            // Hide or show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(m_displayColorbar) scalarBar->SetVisibility(1);

            // Hide or show axis and title of this widget
            if(m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]])
            {
                this->creationAxisWidget(m_selectedIndex[i]);
                this->creationTitleAxisWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetAxis.empty()) this->deleteAxisWidget(m_selectedIndex[i]);
                if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
            }

            // Hide or show sphere and title of this widget
            if(m_displayVectorsByDirection[m_selectedIndex[i]])
            {
                this->creationSphereWidget(m_selectedIndex[i]);
                this->creationTitleSphereWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
                if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
            }

        }
    }
}

void ShapePopulationBase::displayColorMapByDirection(bool display)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display) m_displayColorMapByDirection[m_selectedIndex[i]] = true ;
        else m_displayColorMapByDirection[m_selectedIndex[i]] = false;

        if(display)
        {
            // display of the color map by direction
            ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
            const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
            std::string cmap = std::string(a_cmap);
            std::string key1 ("_mag");
            size_t found = cmap.rfind(key1);
            if (found!=std::string::npos)
                cmap.replace (found,key1.length(),"_ColorByDirection");
            std::string key2 ("_AbsoluteColorByDirection");
            found = cmap.rfind(key2);
            if (found!=std::string::npos)
                cmap.replace (found,key2.length(),"_ColorByDirection");

            // Set Active Scalars for the ColorMap
            mesh->GetPolyData()->GetPointData()->SetActiveScalars(cmap.c_str());

            // Hide or show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(!m_displayVectorsByMagnitude[m_selectedIndex[i]])
            {
                scalarBar->SetVisibility(0);
            }

            // Hide or show sphere
            this->creationSphereWidget(m_selectedIndex[i]);

            if(m_displayVectors[m_selectedIndex[i]])
            {
                // Title of sphere
                if(!m_displayVectorsByDirection[m_selectedIndex[i]])
                {
                    this->creationTitleSphereWidget(m_selectedIndex[i]);
                }
                else
                {
                    if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
                }
                // Hide or show axis and title of this widget
                if(m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]])
                {
                    this->creationTitleAxisWidget(m_selectedIndex[i]);
                }
                else
                {
                    if(!m_widgetAxis.empty()) this->deleteAxisWidget(m_selectedIndex[i]);
                    if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
                }
            }
            else
            {
                if(!m_widgetAxis.empty()) this->deleteAxisWidget(m_selectedIndex[i]);
                if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
            }
        }
    }
}

void ShapePopulationBase::displayAbsoluteColorMapByDirection(bool display)
{   
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display)
        {
            m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]] = true ;
            m_displayColorMapByDirection[m_selectedIndex[i]] = false;
        }
        else m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]] = false;

        if(display)
        {
            // display of the color map by absolute direction
            ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
            const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
            std::string cmap = std::string(a_cmap);
            std::string key1 ("_ColorByDirection");
            size_t found = cmap.rfind(key1);
            if (found!=std::string::npos)
                cmap.replace (found,key1.length(),"_AbsoluteColorByDirection");
            std::string key2 ("_mag");
            found = cmap.rfind(key2);
            if (found!=std::string::npos)
                cmap.replace (found,key2.length(),"_AbsoluteColorByDirection");
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << cmap;

            // Set Active Scalars for the ColorMap
            mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs.str().c_str());

            // Hide or show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(!m_displayVectorsByMagnitude[m_selectedIndex[i]])
            {
                scalarBar->SetVisibility(0);
            }

            // Hide or show the axis/sphere
            this->creationAxisWidget(m_selectedIndex[i]);
            if(m_displayVectors[m_selectedIndex[i]])
            {
                if(!m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]])
                {
                    this->creationTitleAxisWidget(m_selectedIndex[i]);
                }
                else
                {
                    if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
                }
                if(m_displayVectorsByDirection[m_selectedIndex[i]])
                {
                    this->creationTitleSphereWidget(m_selectedIndex[i]);
                }
                else
                {
                    if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
                    if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
                }
            }
            else
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
                if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
            }
        }
    }
}

void ShapePopulationBase::UpdateColorMap(std::vector< unsigned int > a_windowIndex)
{
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        //Look Up table
        vtkSmartPointer<vtkColorTransferFunction> DistanceMapTFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
        double range = fabs(m_usedColorBar->range[1] - m_usedColorBar->range[0]);
        
        for (unsigned int j = 0; j < m_usedColorBar->colorPointList.size(); j++)
        {
            double position = m_usedColorBar->colorPointList[j].pos;
            double x = m_usedColorBar->range[0] + range * position;
            double r = m_usedColorBar->colorPointList[j].r;
            double g = m_usedColorBar->colorPointList[j].g;
            double b = m_usedColorBar->colorPointList[j].b;
            DistanceMapTFunc->AddRGBPoint(x,r,g,b);
        }
        /* The two following lines where there to round the values on the scalarbar
         * but it was also rounding the actual range and therefore the arrow position
         * was restricted to sometime nothing.
         * TODO : either copy the lookuptable and edit it for the scalarbar, either
         * use vtk functions on the scalarbaractor (but bugs during last attempt).
         */
        //m_usedColorBar->range[0] = spv_math::round_nplaces(m_usedColorBar->range[0],2);
        //m_usedColorBar->range[1] = spv_math::round_nplaces(m_usedColorBar->range[1],2);
        DistanceMapTFunc->AdjustRange(m_usedColorBar->range);
        DistanceMapTFunc->SetColorSpaceToRGB();
        DistanceMapTFunc->SetVectorModeToMagnitude(); /*test magnitude to scalars*/
        
        
        //Mesh Mapper Update
        vtkActorCollection * actors = m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
        actors->InitTraversal();
        vtkSmartPointer<vtkMapper> mapper = actors->GetNextActor()->GetMapper();
        mapper->SetLookupTable( DistanceMapTFunc );
        mapper->ScalarVisibilityOn();
        
        //Vector Mapper Update
        vtkSmartPointer<vtkMapper> glyphMapper = actors->GetLastActor()->GetMapper();
        glyphMapper->SetLookupTable( DistanceMapTFunc );
        
        
        //ScalarBar Mapper Update
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)viewPropObject;
        scalarBar->SetLookupTable( DistanceMapTFunc );
        scalarBar->SetTitle(" ");
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            VECTORS                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::setMeshOpacity(double value)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
        actors->InitTraversal();
        actors->GetNextActor()->GetProperty()->SetOpacity(value);
    }
}

void ShapePopulationBase::setVectorScale(double value)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        //        vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
        vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[m_selectedIndex[i]];
        //        glyph->SetSourceConnection(arrow->GetOutputPort());
        glyph->SetScaleFactor(value);
        glyph->Update();
//        if(!m_noUpdateVectorsByDirection)
//        {
            if(m_displayVectorsByDirection[m_selectedIndex[i]] || m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();
//        }
    }
}

void ShapePopulationBase::setVectorDensity(double value)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        vtkSmartPointer<vtkMaskPoints> filter = vtkSmartPointer<vtkMaskPoints>::New();
#if (VTK_MAJOR_VERSION < 6)
        filter->SetInputConnection(mesh->GetPolyData()->GetProducerPort());
#else
        filter->SetInputData(mesh->GetPolyData());
#endif
        filter->SetOnRatio(101-value);
        
        vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[m_selectedIndex[i]];
        glyph->SetInputConnection(filter->GetOutputPort());
        glyph->Update();
//        if(!m_noUpdateVectorsByDirection)
//        {
            if(m_displayVectorsByDirection[m_selectedIndex[i]] || m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();
//        }
    }
}

void ShapePopulationBase::displayVectors(bool display)
{

    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display) m_displayVectors[m_selectedIndex[i]] = true ;
        else m_displayVectors[m_selectedIndex[i]] = false;

        // display of vectors
        const char * a_cmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
        std::string cmap = std::string(a_cmap);
        std::string key1 ("_mag");
        size_t found = cmap.rfind(key1);
        std::string new_cmap = cmap.substr(0,found);
        
        std::string key2 ("_AbsoluteColorByDirection");
        found = cmap.rfind(key2);
        new_cmap = new_cmap.substr(0,found);
        
        std::string key3 ("_ColorByDirection");
        found = cmap.rfind(key3);
        new_cmap = new_cmap.substr(0,found);
        
        if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()))
        {
            vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
            vtkSmartPointer<vtkActor> glyphActor = actors->GetLastActor();
            if(display) glyphActor->SetVisibility(1);
            else glyphActor->SetVisibility(0);
        }
        
        // Hide or show the scalar bar
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)viewPropObject;
        if(!m_displayVectors[m_selectedIndex[i]])
        {
            if(m_displayColorMapByDirection[m_selectedIndex[i]] || m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]]) scalarBar->SetVisibility(0);
            else if(m_displayColorMapByMagnitude[m_selectedIndex[i]])
            {
                if(m_displayColorbar) scalarBar->SetVisibility(1);
            }
        }
        else
        {
            if(m_displayColorMapByMagnitude[m_selectedIndex[i]] || m_displayVectorsByMagnitude[m_selectedIndex[i]])
            {
                if(m_displayColorbar) scalarBar->SetVisibility(1);
            }
            else
            {
                scalarBar->SetVisibility(0);
            }
        }

        // Hide or show the axis/sphere
        if(m_displayVectors[m_selectedIndex[i]])
        {
            if(m_displayColorMapByDirection[m_selectedIndex[i]] || m_displayVectorsByDirection[m_selectedIndex[i]])
            {
                this->creationSphereWidget(m_selectedIndex[i]);
                if(!(m_displayColorMapByDirection[m_selectedIndex[i]] && m_displayVectorsByDirection[m_selectedIndex[i]]))
                {
                    this->creationTitleSphereWidget(m_selectedIndex[i]);
                }
            }
            if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]] || m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]])
            {
                this->creationAxisWidget(m_selectedIndex[i]);
                if(!(m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]] && m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]]))
                {
                    this->creationTitleAxisWidget(m_selectedIndex[i]);
                }
            }
        }
        else // delete of vectors
        {
            // Hide axis
            if(!m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]])
            {
                if(!m_widgetAxis.empty()) this->deleteAxisWidget(m_selectedIndex[i]);
            }

            // Hide sphere
            if(!m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
            }
            // delete of titles of axis and sphere
            if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
            if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
        }
    }
}

void ShapePopulationBase::displayVectorsByMagnitude(bool display)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display) m_displayVectorsByMagnitude[m_selectedIndex[i]] = true ;
        else m_displayVectorsByMagnitude[m_selectedIndex[i]] = false;

        if(display)
        {
            // diplay of the color of vectors by magnitude
            const char * a_cmap = m_meshList[m_selectedIndex[i]]->GetPolyData()->GetPointData()->GetScalars()->GetName();
            std::string cmap = std::string(a_cmap);
            size_t found = cmap.rfind("_mag");
            std::string new_cmap = cmap.substr(0,found);
            found = cmap.rfind("_ColorByDirection");
            new_cmap = new_cmap.substr(0,found);
            found = cmap.rfind("_AbsoluteColorByDirection");
            new_cmap = new_cmap.substr(0,found);

            if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()) )
            {
                vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
                vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[m_selectedIndex[i]];
                glyph->SetSourceConnection(arrow->GetOutputPort());
                glyph->SetColorModeToColorByVector();
                glyph->Update();

                vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
                vtkSmartPointer<vtkActor> glyphActor = actors->GetLastActor();

                if(display) glyphActor->SetVisibility(1);
                else glyphActor->SetVisibility(0);
            }
            // Hide or show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(m_displayColorbar) scalarBar->SetVisibility(1);

            // Hide or show axis and title of this widget
            if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationAxisWidget(m_selectedIndex[i]);
                this->creationTitleAxisWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetAxis.empty()) this->deleteAxisWidget(m_selectedIndex[i]);
                if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
            }

            // Hide or show sphere and title of this widget
            if(m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationSphereWidget(m_selectedIndex[i]);
                this->creationTitleSphereWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
                if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
            }
        }
    }
}

void ShapePopulationBase::displayVectorsByDirection(bool display)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display) m_displayVectorsByDirection[m_selectedIndex[i]] =  true;
        else m_displayVectorsByDirection[m_selectedIndex[i]] = false;

        if(display)
        {
            //display of the color of vectors by direction
            ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
            const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
            std::string cmap = std::string(a_cmap);
            size_t found = cmap.rfind("_mag");
            std::string new_cmap = cmap.substr(0,found);
            found = cmap.rfind("_ColorByDirection");
            new_cmap = new_cmap.substr(0,found);
            found = cmap.rfind("_AbsoluteColorByDirection");
            new_cmap = new_cmap.substr(0,found);
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << new_cmap;

            if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()) )
            {

                if(m_displayColorMapByDirection[m_selectedIndex[i]])
                {
                    mesh->GetPolyData()->GetPointData()->SetActiveVectors(strs.str().c_str());

                    vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
                    vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[m_selectedIndex[i]];
                    glyph->SetSourceConnection(arrow->GetOutputPort());
                    glyph->SetColorModeToColorByScalar();
                    glyph->Update();

                }
                else if (m_displayColorMapByMagnitude[m_selectedIndex[i]] || m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();

                vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
                vtkSmartPointer<vtkActor> glyphActor = actors->GetLastActor();
                if(display) glyphActor->SetVisibility(1);
                else glyphActor->SetVisibility(0);
            }

            // Hide or show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(m_displayColorMapByDirection[m_selectedIndex[i]] || m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]])
            {
                scalarBar->SetVisibility(0);
            }
            else if (m_displayColorMapByMagnitude[m_selectedIndex[i]] && m_displayColorbar)
            {
                scalarBar->SetVisibility(1);
            }

            // Show sphere
            this->creationSphereWidget(m_selectedIndex[i]);
            // Title of this widget
            if(!m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationTitleSphereWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
            }

            // Hide or show axis and title of this widget
            if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationAxisWidget(m_selectedIndex[i]);
                this->creationTitleAxisWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetAxis.empty()) this->deleteAxisWidget(m_selectedIndex[i]);
                if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
            }

        }
    }
}

void ShapePopulationBase::displayVectorsByAbsoluteDirection(bool display)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        if(display) m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]] = true;
        else m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]] = false;

        if(display)
        {
            //display of the color of vectors by direction
            ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
            const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
            std::string cmap = std::string(a_cmap);
            size_t found = cmap.rfind("_mag");
            std::string new_cmap = cmap.substr(0,found);
            found = cmap.rfind("_ColorByDirection");
            new_cmap = new_cmap.substr(0,found);
            found = cmap.rfind("_AbsoluteColorByDirection");
            new_cmap = new_cmap.substr(0,found);
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << new_cmap;

            if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()) )
            {

                if(m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]])
                {
                    mesh->GetPolyData()->GetPointData()->SetActiveVectors(strs.str().c_str());

                    vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
                    vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[m_selectedIndex[i]];
                    glyph->SetSourceConnection(arrow->GetOutputPort());
                    glyph->SetColorModeToColorByScalar();
                    glyph->Update();

                }
                else if (m_displayColorMapByMagnitude[m_selectedIndex[i]] || m_displayColorMapByDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();

                vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
                vtkSmartPointer<vtkActor> glyphActor = actors->GetLastActor();
                if(display) glyphActor->SetVisibility(1);
                else glyphActor->SetVisibility(0);
            }

            // Hide or show the scalar bar
            vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar = (vtkScalarBarActor*)viewPropObject;
            if(m_displayColorMapByDirection[m_selectedIndex[i]] || m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]]) scalarBar->SetVisibility(0);
            else if (m_displayColorMapByMagnitude[m_selectedIndex[i]] && m_displayColorbar) scalarBar->SetVisibility(1);


            // Show axis
            this->creationAxisWidget(m_selectedIndex[i]);
            // Title of this widget
            if(!m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationTitleAxisWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetTitleAxis.empty()) this->deleteTitleAxisWidget(m_selectedIndex[i]);
            }

            // Hide or show sphere  and title of this widget
            if(m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationSphereWidget(m_selectedIndex[i]);
                this->creationTitleSphereWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
                if(!m_widgetTitleSphere.empty()) this->deleteTitleSphereWidget(m_selectedIndex[i]);
            }

        }
    }
}

void ShapePopulationBase::UpdateVectorsByDirection()
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        const char * a_cmap = mesh->GetPolyData()->GetPointData()->GetScalars()->GetName();
        
        std::string cmap = std::string(a_cmap);
        size_t found = cmap.rfind("_mag");
        std::string new_cmap = cmap.substr(0,found);
        found = cmap.rfind("_ColorByDirection");
        new_cmap = new_cmap.substr(0,found);
        found = cmap.rfind("_AbsoluteColorByDirection");
        new_cmap = new_cmap.substr(0,found);
        std::ostringstream strs;
        strs.str(""); strs.clear();
        strs << new_cmap;
        std::ostringstream strs_mag;
        strs_mag.str(""); strs_mag.clear();
        strs_mag << new_cmap << "_mag" << std::endl;
        std::ostringstream strs_dir;
        strs_dir.str(""); strs_dir.clear();
        strs_dir << new_cmap << "_ColorByDirection" << std::endl;
        std::ostringstream strs_abs;
        strs_abs.str(""); strs_abs.clear();
        strs_abs << new_cmap << "_AbsoluteColorByDirection" << std::endl;
        
        // Set Active Scalars to color vectors
        if(m_displayVectorsByDirection[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_dir.str().c_str());
        else if(m_displayVectorsByAbsoluteDirection[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_abs.str().c_str());

        // Set Active Vectors
        mesh->GetPolyData()->GetPointData()->SetActiveVectors(strs.str().c_str());
        
        vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
        vtkSmartPointer<vtkGlyph3D> glyph = m_glyphList[m_selectedIndex[i]];
        glyph->SetSourceConnection(arrow->GetOutputPort());
        glyph->SetColorModeToColorByScalar();
        glyph->Update();
        
        // Set Active Scalars to re-color the colormap
        if(m_displayColorMapByMagnitude[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_mag.str().c_str());
        else if (m_displayColorMapByDirection[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_dir.str().c_str());
        else if (m_displayAbsoluteColorMapByDirection[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_abs.str().c_str());
    }
    
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            DISPLAY                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::displayColorbar(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        
        // cornerAnnotation
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)viewPropObject;
        
        if(display)
        {
            m_displayColorbar = true;
            if(m_displayColorMapByMagnitude[i] || m_displayVectorsByMagnitude[i]) scalarBar->SetVisibility(1);
        }
        else
        {
            m_displayColorbar = false;
            scalarBar->SetVisibility(0);
        }
    }
}

void ShapePopulationBase::displayAttribute(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        
        // cornerAnnotation
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        
        if(display)
        {
            m_displayAttribute = true;
            cornerAnnotation->SetVisibility(1);
        }
        else
        {
            m_displayAttribute = false;
            cornerAnnotation->SetVisibility(0);
        }
    }
}

void ShapePopulationBase::displayMeshName(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        vtkSmartPointer<vtkPropCollection> propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        
        // cornerAnnotation
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
        cornerAnnotation = (vtkCornerAnnotation*) viewPropObject;
        
        if(display)
        {
            m_displayMeshName = true;
            cornerAnnotation->SetVisibility(1);
        }
        else
        {
            m_displayMeshName = false;
            cornerAnnotation->SetVisibility(0);
        }
    }
}

void ShapePopulationBase::displayAxis(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        if(display)
        {
            m_displayAxis = true;
            if(m_displayAbsoluteColorMapByDirection[i] || m_displayVectorsByAbsoluteDirection[i])
            {
                creationAxisWidget(i);
                if(m_displayVectors[i])
                {
                    if(!(m_displayAbsoluteColorMapByDirection[i] && m_displayVectorsByAbsoluteDirection[i]))
                    {
                        creationTitleAxisWidget(i);
                    }
                }
            }
        }
        else
        {
            m_displayAxis = false;
            if(!m_widgetAxis.empty()) deleteAxisWidget(i);
            if(!m_widgetTitleAxis.empty()) deleteTitleAxisWidget(i);
        }
    }
}

void ShapePopulationBase::displaySphere(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        if(display)
        {
            m_displaySphere = true;
            if(m_displayColorMapByDirection[i] || m_displayVectorsByDirection[i])
            {
                creationSphereWidget(i);
                if(m_displayVectors[i])
                {
                    if(!(m_displayColorMapByDirection[i] && m_displayVectorsByDirection[i]))
                    {
                        creationTitleSphereWidget(i);
                    }
                }
            }
        }
        else
        {
            m_displaySphere = false;
            if(!m_widgetSphere.empty()) deleteSphereWidget(i);
            if(!m_widgetTitleSphere.empty()) deleteTitleSphereWidget(i);
        }
    }
}

void ShapePopulationBase::displayTitles(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        if(display)
        {
            m_displayTitles = true;
            if(m_displayColorMapByDirection[i] || m_displayVectorsByDirection[i])
            {
                if(m_displayVectors[i])
                {
                    if(!(m_displayColorMapByDirection[i] && m_displayVectorsByDirection[i]))
                    {
                        creationTitleSphereWidget(i);
                    }
                }
            }
            if(m_displayAbsoluteColorMapByDirection[i] || m_displayVectorsByAbsoluteDirection[i])
            {
                if(m_displayVectors[i])
                {
                    if(!(m_displayAbsoluteColorMapByDirection[i] && m_displayVectorsByAbsoluteDirection[i]))
                    {
                        creationTitleAxisWidget(i);
                    }
                }
            }

        }
        else
        {
            m_displayTitles = false;
            if(!m_widgetTitleAxis.empty()) deleteTitleAxisWidget(i);
            if(!m_widgetTitleSphere.empty()) deleteTitleSphereWidget(i);
        }
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            AXIS                                               * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //


void ShapePopulationBase::creationAxisWidget(int index)
{
    if(m_displayAxis)
    {
        vtkRenderWindow *renderWindow = m_windowsList[index];

        vtkRenderWindowInteractor *iren = renderWindow->GetInteractor();

        vtkSmartPointer<vtkAxesActor> actorAxis = vtkSmartPointer<vtkAxesActor>::New();

        vtkOrientationMarkerWidget* widgetAxis = vtkOrientationMarkerWidget::New();
        widgetAxis = m_widgetAxis[index];
        widgetAxis->SetOutlineColor( 1, 1, 1 ); // color for the frame around the axes
        widgetAxis->SetOrientationMarker( actorAxis );
        widgetAxis->SetInteractor( iren );
        widgetAxis->SetViewport( 0.0, 0.05, 0.15, 0.3 ); // size and position of the frame
        widgetAxis->SetEnabled( 1 );
        widgetAxis->InteractiveOff();

        m_createAxis[index] = true;
    }
}

void ShapePopulationBase::deleteAxisWidget(int index)
{
    if(m_createAxis[index])
    {
        m_widgetAxis[index]->SetEnabled( 0 );
        m_widgetAxis[index]->Delete();
        vtkOrientationMarkerWidget* widgetAxis = vtkOrientationMarkerWidget::New();
        m_widgetAxis[index] = widgetAxis;
    }
    m_createAxis[index] = false;

}

void ShapePopulationBase::creationTitleAxisWidget(int index)
{
    if(m_displayTitles && m_displayAxis)
    {
        vtkRenderWindow *renderWindow = m_windowsList[index];

        vtkRenderWindowInteractor *iren = renderWindow->GetInteractor();

        // TITLE
        vtkSmartPointer<vtkTextActor> actorTitleAxis = vtkSmartPointer<vtkTextActor>::New();
        actorTitleAxis->GetTextProperty()->SetFontSize ( 14 );
        if(m_displayAbsoluteColorMapByDirection[index] && !m_displayVectorsByAbsoluteDirection[index]) actorTitleAxis->SetInput( "Color Map" );
        else if(!m_displayAbsoluteColorMapByDirection[index] && m_displayVectorsByAbsoluteDirection[index]) actorTitleAxis->SetInput( "Color of vectors" );
        actorTitleAxis->GetTextProperty()->SetColor ( m_labelColor );

        vtkOrientationMarkerWidget* widgetTitleAxis = vtkOrientationMarkerWidget::New();
        widgetTitleAxis = m_widgetTitleAxis[index];
        widgetTitleAxis->SetOutlineColor( 1, 1, 1 ); // color for the frame around the axes
        widgetTitleAxis->SetOrientationMarker( actorTitleAxis );
        widgetTitleAxis->SetInteractor( iren );
        widgetTitleAxis->SetViewport( 0.05, 0.3, 0.5, 0.4 ); // size and position of the frame
        widgetTitleAxis->SetEnabled( 1 );
        widgetTitleAxis->InteractiveOff();

        m_createTitleAxis[index] = true;
    }
}

void ShapePopulationBase::deleteTitleAxisWidget(int index)
{
    if(m_createTitleAxis[index])
    {
        m_widgetTitleAxis[index]->SetEnabled( 0 );
        m_widgetTitleAxis[index]->Delete();
        vtkOrientationMarkerWidget* widgetTitleAxis = vtkOrientationMarkerWidget::New();
        m_widgetTitleAxis[index] = widgetTitleAxis;
    }
    m_createTitleAxis[index] = false;
}





// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            SHPERE                                             * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
vtkActor* ShapePopulationBase::creationSphereActor()
{
    // Create a sphere
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5.0);
    sphereSource->SetThetaResolution(40);
    sphereSource->SetPhiResolution(40);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyData> polyData = sphereSource->GetOutput();

    // Color map by direction
    vtkIdType numPts = polyData->GetNumberOfPoints();
    vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
    scalars->SetNumberOfComponents(3);
    scalars->SetNumberOfTuples(numPts);
    scalars->SetName("ColorByDirection");

    // minimum and maximum
    double min[3];
    double max[3];
    double p[3];
    for(int j = 0; j < 3; j++)
    {
        std::vector<double> vectXYZ;
        for(vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++)
        {
            polyData->GetPoint(i,p);
            vectXYZ.push_back(p[j]);
        }
        std::sort (vectXYZ.begin(), vectXYZ.end()); // using default comparison (operator <)
        min[j] = vectXYZ[0];
        max[j] = vectXYZ[numPts-1];
    }
    // RGB scalar corresponding
    double RGB[3];
    for( vtkIdType i = 0; i < numPts; ++i )
    {
        polyData->GetPoint(i,p);
        for(int k = 0; k < 3; k++)
        {
            if(max[k] != min[k]) RGB[k] = ((255/(max[k]-min[k]))*(p[k])-(255*min[k])/(max[k]-min[k]));
            else if (max[k] != 0) RGB[k] = 255;
            else if (max[k] == 0) RGB[k] = 0;
        }
        scalars->InsertTuple(i,RGB);
    }
    polyData->GetPointData()->SetScalars(scalars);

    // Map of the downloaded figure
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if (VTK_MAJOR_VERSION < 6)
    mapper->SetInputConnection(polyData->GetOutputPort());
#else
    mapper->SetInputData(polyData);
#endif

    // Actor of the downloaded figure
    vtkActor* actorSphere = vtkActor::New();
    actorSphere->SetMapper(mapper);

    return actorSphere;
}

void ShapePopulationBase::creationSphereWidget(int index)
{
    if(m_displaySphere)
    {
        vtkRenderWindow *renderWindow = m_windowsList[index];

        vtkRenderWindowInteractor *iren = renderWindow->GetInteractor();

        // SPHERE WITH GRADIENT COLORS
        vtkSmartPointer<vtkActor> actorSphere;
        actorSphere.TakeReference(creationSphereActor());

        vtkOrientationMarkerWidget* widgetSphere = vtkOrientationMarkerWidget::New();
        widgetSphere = m_widgetSphere[index];
        widgetSphere->SetOrientationMarker( actorSphere );
        widgetSphere->SetInteractor( iren );
        widgetSphere->SetViewport( 0.0, 0.40, 0.15, 0.65 ); // size and position of the frame
        widgetSphere->SetEnabled( 1 );
        widgetSphere->InteractiveOff();


        // AXIS
        vtkSmartPointer<vtkAxesActor> actorAxisByDirection = vtkSmartPointer<vtkAxesActor>::New();

        // color of axis
        actorAxisByDirection->GetXAxisShaftProperty()->SetColor(1,0.5,0.5);
        actorAxisByDirection->GetXAxisTipProperty()->SetColor(1,0.5,0.5);
        actorAxisByDirection->GetYAxisShaftProperty()->SetColor(0.5,1,0.5);
        actorAxisByDirection->GetYAxisTipProperty()->SetColor(0.5,1,0.5);
        actorAxisByDirection->GetZAxisShaftProperty()->SetColor(0.5,0.5,1);
        actorAxisByDirection->GetZAxisTipProperty()->SetColor(0.5,0.5,1);

        vtkOrientationMarkerWidget* widgetAxisByDirection = vtkOrientationMarkerWidget::New();
        widgetAxisByDirection = m_widgetAxisByDirection[index];
        widgetAxisByDirection->SetOrientationMarker( actorAxisByDirection );
        widgetAxisByDirection->SetInteractor( iren );
        widgetAxisByDirection->SetViewport( 0.0, 0.55, 0.15, 0.80  ); // size and position of the frame
        widgetAxisByDirection->SetEnabled( 1 );
        widgetAxisByDirection->InteractiveOff();

        m_createSphere[index] = true;
    }
}

void ShapePopulationBase::deleteSphereWidget(int index)
{
    if(m_createSphere[index])
    {
        m_widgetSphere[index]->SetEnabled( 0 );
        m_widgetSphere[index]->Delete();
        m_widgetAxisByDirection[index]->SetEnabled( 0 );
        m_widgetAxisByDirection[index]->Delete();
        vtkOrientationMarkerWidget* widgetSphere = vtkOrientationMarkerWidget::New();
        m_widgetSphere[index] = widgetSphere;
        vtkOrientationMarkerWidget* widgetAxisByDirection = vtkOrientationMarkerWidget::New();
        m_widgetAxisByDirection[index] = widgetAxisByDirection;
    }
    m_createSphere[index] = false;
}


void ShapePopulationBase::creationTitleSphereWidget(int index)
{
    if(m_displayTitles && m_displaySphere)
    {
        vtkRenderWindow *renderWindow = m_windowsList[index];

        vtkRenderWindowInteractor *iren = renderWindow->GetInteractor();

        // TITLE
        vtkSmartPointer<vtkTextActor> actorTitleSphere = vtkSmartPointer<vtkTextActor>::New();
        actorTitleSphere->GetTextProperty()->SetFontSize ( 14 );
        if(m_displayColorMapByDirection[index] && !m_displayVectorsByDirection[index]) actorTitleSphere->SetInput ( "Color Map" );
        else if(!m_displayColorMapByDirection[index] && m_displayVectorsByDirection[index]) actorTitleSphere->SetInput ( "Color of vectors" );
        actorTitleSphere->GetTextProperty()->SetColor ( m_labelColor );

        vtkOrientationMarkerWidget* widgetTitleSphere = vtkOrientationMarkerWidget::New();
        widgetTitleSphere = m_widgetTitleSphere[index];
        widgetTitleSphere->SetOutlineColor( 1, 1, 1 ); // color for the frame around the axes
        widgetTitleSphere->SetOrientationMarker( actorTitleSphere );
        widgetTitleSphere->SetInteractor( iren );
        widgetTitleSphere->SetViewport( 0.05, 0.80, 0.5, 0.90 ); // size and position of the frame
        widgetTitleSphere->SetEnabled( 1 );
        widgetTitleSphere->InteractiveOff();

        m_createTitleSphere[index] = true;
    }
}

void ShapePopulationBase::deleteTitleSphereWidget(int index)
{
    if(m_createTitleSphere[index])
    {
        m_widgetTitleSphere[index]->SetEnabled( 0 );
        m_widgetTitleSphere[index]->Delete();
        vtkOrientationMarkerWidget* widgetTitleSphere = vtkOrientationMarkerWidget::New();
        m_widgetTitleSphere[index] = widgetTitleSphere;
    }
    m_createTitleSphere[index] = false;
}

void ShapePopulationBase::deleteAllWidgets()
{
    for (unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        if(!m_widgetAxis.empty()) deleteAxisWidget(i);
        if(!m_widgetTitleAxis.empty())deleteTitleAxisWidget(i);
        if(!m_widgetSphere.empty())deleteSphereWidget(i);
        if(!m_widgetTitleSphere.empty())deleteTitleSphereWidget(i);
    }
}

void ShapePopulationBase::initializationAllWidgets()
{
    m_widgetAxis.clear();
    m_createAxis.clear();

    m_widgetSphere.clear();
    m_widgetAxisByDirection.clear();
    m_createSphere.clear();

    m_widgetTitleAxis.clear();
    m_createTitleAxis.clear();

    m_widgetTitleSphere.clear();
    m_createTitleSphere.clear();

    // initialization of all the widgets
    for (unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        vtkOrientationMarkerWidget* widgetAxis = vtkOrientationMarkerWidget::New();
        m_widgetAxis.push_back(widgetAxis);
        m_createAxis.push_back(false);

        vtkOrientationMarkerWidget* widgetSphere = vtkOrientationMarkerWidget::New();
        vtkOrientationMarkerWidget* widgetAxisByDirection = vtkOrientationMarkerWidget::New();

        m_widgetSphere.push_back(widgetSphere);
        m_widgetAxisByDirection.push_back(widgetAxisByDirection);
        m_createSphere.push_back(false);

        vtkOrientationMarkerWidget* widgetTitleAxis = vtkOrientationMarkerWidget::New();

        m_widgetTitleAxis.push_back(widgetTitleAxis);
        m_createTitleAxis.push_back(false);

        vtkOrientationMarkerWidget* widgetTitleSphere = vtkOrientationMarkerWidget::New();

        m_widgetTitleSphere.push_back(widgetTitleSphere);
        m_createTitleSphere.push_back(false);
    }
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            CAMERA                                             * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::ChangeView(int x, int y, int z)
{
    vtkSmartPointer<vtkRenderer> firstRenderer = vtkSmartPointer<vtkRenderer>::New();
    firstRenderer = m_windowsList[m_selectedIndex[0]]->GetRenderers()->GetFirstRenderer();
    
    double *coords  = firstRenderer->GetActiveCamera()->GetFocalPoint();
    double distance = firstRenderer->GetActiveCamera()->GetDistance();
    firstRenderer->GetActiveCamera()->SetPosition(coords[0]+x*distance,coords[1]+y*distance,coords[2]+z*distance);
    
    
    //setroll to .001, because it breaks on y axis if roll = 0
    firstRenderer->GetActiveCamera()->SetRoll(.001);
    
    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
    this->UpdateCameraConfig();
    
}

void ShapePopulationBase::ResetHeadcam()
{
    
    if(m_selectedIndex.empty())
    {
        vtkSmartPointer<vtkRenderer> firstRenderer = m_windowsList[0]->GetRenderers()->GetFirstRenderer();
        firstRenderer->ResetCamera();
        m_headcam->DeepCopy(firstRenderer->GetActiveCamera());
    }
    vtkSmartPointer<vtkRenderer> firstRenderer = m_windowsList[m_selectedIndex[0]]->GetRenderers()->GetFirstRenderer();
    firstRenderer->ResetCamera();
    this->UpdateCameraConfig();
    
}

void ShapePopulationBase::UpdateCameraConfig()
{
    double * position = m_headcam->GetPosition();
    m_headcamConfig.pos_x = position[0];
    m_headcamConfig.pos_y = position[1];
    m_headcamConfig.pos_z = position[2];
    double * focalpoint = m_headcam->GetFocalPoint();
    m_headcamConfig.foc_x = focalpoint[0];
    m_headcamConfig.foc_y = focalpoint[1];
    m_headcamConfig.foc_z = focalpoint[2];
    double * viewup = m_headcam->GetViewUp();
    m_headcamConfig.view_vx = viewup[0];
    m_headcamConfig.view_vy = viewup[1];
    m_headcamConfig.view_vz = viewup[2];
    m_headcamConfig.scale = m_headcam->GetParallelScale();
}

void ShapePopulationBase::AlignMesh(bool alignment)
{
    if(alignment == true)
    {
        for (unsigned int i = 0; i < m_windowsList.size();i++)
        {
            //Get the actual position
            vtkActorCollection * actors = m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetActors();
            actors->InitTraversal();
            vtkSmartPointer<vtkActor> meshActor = actors->GetNextActor();
            vtkSmartPointer<vtkActor> glyphActor = actors->GetLastActor();
            double * position = meshActor->GetPosition();
            double * center = meshActor->GetCenter();
            
            //Calculate the new position
            double a = position[0]-center[0];
            double b = position[1]-center[1];
            double c = position[2]-center[2];
            double newposition[3] = {a,b,c};
            
            //Update the position
            meshActor->SetPosition(newposition);
            glyphActor->SetPosition(newposition);
        }
    }
    else
    {
        for (unsigned int i = 0; i < m_windowsList.size();i++)
        {
            //Get the position
            vtkActorCollection * actors = m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetActors();
            actors->InitTraversal();
            vtkSmartPointer<vtkActor> meshActor = actors->GetNextActor();
            vtkSmartPointer<vtkActor> glyphActor = actors->GetLastActor();
            
            //Update the position
            double newposition[3] = {0,0,0};
            meshActor->SetPosition(newposition);
            glyphActor->SetPosition(newposition);
        }
    }
    
    this->ResetHeadcam();
    
    for (unsigned int i = 0; i < m_windowsList.size();i++)
    {
        vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
        camera->DeepCopy(m_headcam);
        m_windowsList[i]->GetRenderers()->GetFirstRenderer()->SetActiveCamera(camera);
    }
    for (unsigned int i = 0; i < m_selectedIndex.size();i++)
    {
        m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->SetActiveCamera(m_headcam);
    }
}

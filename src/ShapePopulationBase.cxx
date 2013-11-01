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

    for(unsigned int i = 0; i < m_windowsList.size(); i++)
    {
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
    mapper->SetInputConnection(Mesh->GetPolyData()->GetProducerPort());

    //ACTOR
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    //RENDERER
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetActiveCamera(m_headcam); //set the active camera for this renderer to main camera
    renderer->ResetCamera();

    //WINDOW
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    m_windowsList.push_back(renderWindow);

    //INTERACTOR
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    //ANNOTATIONS
    vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
    cornerAnnotation->SetLinearFontScaleFactor( 2 );
    cornerAnnotation->SetNonlinearFontScaleFactor( 1 );
    cornerAnnotation->SetMaximumFontSize( 15 );
    cornerAnnotation->SetText( 2,Mesh->GetFileName().c_str());
    renderer->AddViewProp(cornerAnnotation);

    // SCALAR BAR
    vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetNumberOfLabels(5);
    scalarBar->SetMaximumWidthInPixels(60);

    vtkSmartPointer<vtkTextProperty> LabelProperty = vtkSmartPointer<vtkTextProperty>::New();
    LabelProperty->SetFontSize(12);
    scalarBar->SetLabelTextProperty(LabelProperty);

    renderer->AddActor2D(scalarBar);

/*  //TEST VTK without GUI
    renderWindow->Render();
    interactor->Start();
    */

}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                          SELECTION                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

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
        if(selectedInteractor->GetControlKey()==1)
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
    if(selectedInteractor->GetControlKey()==0)
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
        const char * cmap = m_windowsList[m_selectedIndex[0]]->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput()->GetPointData()->GetScalars()->GetName();
        this->UpdateAttribute(cmap, m_selectedIndex);
        this->UpdateColorMap(m_selectedIndex);

        //Update Center to the first selected window position
        std::string position = m_meshList[m_selectedIndex[0]]->GetPosition();

        if(strcmp(position.c_str(),"Centered") == 0)
        {
            this->PositionToCentered();
        }
        else if(strcmp(position.c_str(),"Original") == 0)
        {
            this->PositionToOriginal();
        }
    }

    this->RenderSelection();
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
        this->RenderSelection();
    }
    //SELECT ALL - GetControlKey != 0 & GetKeySym()="a"
    else if(selectedInteractor->GetControlKey() != 0 && strcmp(selectedInteractor->GetKeySym(),"a") == 0)
    {
        SelectAll();
        this->RenderSelection();
    }
}

void ShapePopulationBase::CameraChangedEventVTK(vtkObject*, unsigned long, void*)
{
    this->UpdateCameraConfig();
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                           RENDERING                                           * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::RenderSelection()
{
    if(m_selectedIndex.size()==0) return;

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
            m_windowsList.at(i)->RemoveAllObservers();
            m_windowsList.at(i)->AddObserver(vtkCommand::RenderEvent, this, &ShapePopulationBase::RenderSelection);
        }
    }
    if(!realtime)
    {
        for (unsigned int i = 0; i < m_windowsList.size(); i++)
        {
            //syncronize when render modified
            m_windowsList.at(i)->RemoveAllObservers();
            m_windowsList.at(i)->AddObserver(vtkCommand::ModifiedEvent, this, &ShapePopulationBase::RenderSelection);
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
    double * commonRange = m_commonRange; //to avoid warning for not being initialized
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        double * newRange = m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput()->GetPointData()->GetScalars(a_cmap)->GetRange();

        if(i==0) commonRange = newRange;
        else
        {
            if(newRange[0] < commonRange[0]) commonRange[0] = newRange[0];
            if(newRange[1] > commonRange[1]) commonRange[1] = newRange[1];
        }
    }

    return commonRange;
}
void ShapePopulationBase::UpdateAttribute(const char * a_cmap, std::vector< unsigned int > a_windowIndex)
{
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        vtkSmartPointer<vtkMapper> mapper = m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper();
        mapper->GetInput()->GetPointData()->SetActiveScalars(a_cmap);
    }

    // Compute the largest range
    m_commonRange = computeCommonRange(a_cmap, a_windowIndex);
}

void ShapePopulationBase::UpdateColorMap(std::vector< unsigned int > a_windowIndex)
{

    // Set Attribute, Calculate & Apply the Look Up Table
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        vtkSmartPointer<vtkMapper> mapper = m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper();
        vtkSmartPointer<vtkColorTransferFunction> DistanceMapTFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
        double range = fabs(m_commonRange[1] - m_commonRange[0]);

        for (unsigned int j = 0; j < m_colorPointList->size(); j++)
        {
            double position = m_colorPointList->at(j).pos;
			double x = spv_math::round_nplaces(m_commonRange[0] + range * position,2);
            double r = m_colorPointList->at(j).r;
            double g = m_colorPointList->at(j).g;
            double b = m_colorPointList->at(j).b;
            DistanceMapTFunc->AddRGBPoint(x,r,g,b);
        }
        m_commonRange[0] = spv_math::round_nplaces(m_commonRange[0],2);
        m_commonRange[1] = spv_math::round_nplaces(m_commonRange[1],2);
        DistanceMapTFunc->AdjustRange(m_commonRange);
        DistanceMapTFunc->SetColorSpaceToRGB();

        //Mapper Update
        mapper->SetLookupTable( DistanceMapTFunc );
        mapper->ScalarVisibilityOn();
        mapper->Update();

        //ScalarBar Update
        vtkActor2D * oldScalarBar = m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors2D()->GetLastActor2D();
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar = (vtkScalarBarActor*)oldScalarBar;
        scalarBar->SetLookupTable(DistanceMapTFunc);
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

    this->RenderSelection();
    this->UpdateCameraConfig();

}

void ShapePopulationBase::ResetHeadcam()
{
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

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            CENTER                                             * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::PositionToOriginal()
{
    if(m_selectedIndex.empty()) return;

    for (unsigned int i = 0; i < m_selectedIndex.size();i++)
    {
        //Get the position
        vtkSmartPointer<vtkActor> testActor = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();

        //Update the position
        double newposition[3] = {0,0,0};
        testActor->SetPosition(newposition);
        testActor->SetOrigin(newposition);

        //Update the mesh info
        m_meshList[m_selectedIndex[i]]->SetPosition("Original");
    }

    this->ResetHeadcam();
    this->UpdateCameraConfig();
}

void ShapePopulationBase::PositionToCentered()
{
    if(m_selectedIndex.empty()) return;

    for (unsigned int i = 0; i < m_selectedIndex.size();i++)
    {
        //Get the position
        vtkSmartPointer<vtkActor> testActor = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        double * position = testActor->GetPosition();
        double * center = testActor->GetCenter();

        //Update the position
        double a = position[0]-center[0];
        double b = position[1]-center[1];
        double c = position[2]-center[2];
        double newposition[3] = {a,b,c};
        testActor->SetPosition(newposition);
        testActor->SetOrigin(newposition);

        //Update the mesh info
        m_meshList[m_selectedIndex[i]]->SetPosition("Centered");
    }

    this->ResetHeadcam();
    this->UpdateCameraConfig();
}

unsigned int ShapePopulationBase::getSelectedIndex(vtkSmartPointer<vtkRenderWindow> a_selectedWindow)
{
    unsigned int j=0;
    for( j=0 ; j < m_windowsList.size() ; j++)
    {
        if (a_selectedWindow == m_windowsList[j]) break;
    }
    return j;
}

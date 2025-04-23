#include "ShapePopulationBase.h"

#include <vtkGenericOpenGLRenderWindow.h>

// STD includes
#include <iterator>

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
    m_renderAllSelection = false; //changed
    m_displayColorbar = true;
    m_displayAttribute = true;
    m_displayMeshName = true;
    m_displaySphere = true;
    m_noUpdateVectorsByDirection = false;
    m_createSphere.push_back(false);
}

ShapePopulationBase::~ShapePopulationBase()
{
  for (unsigned int i = 0; i < m_meshList.size(); i++)
    {
    delete m_meshList.at(i);
    }
  m_meshList.clear();
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
        vtkPropCollection* propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();

        //CornerAnnotation Update
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);
        vtkTextProperty* cornerProperty = cornerAnnotation->GetTextProperty();
        cornerProperty->SetColor(m_labelColor);

        //CornerAnnotation Update
        viewPropObject = propCollection->GetItemAsObject(3);
        cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);
        cornerProperty = cornerAnnotation->GetTextProperty();
        cornerProperty->SetColor(m_labelColor);

        //ScalarBar Update
        viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        vtkTextProperty* labelProperty = scalarBar->GetLabelTextProperty();
        labelProperty->SetColor(m_labelColor);

        // Title of the sphere and caption of axis
        if(!m_widgetSphere.empty()) deleteSphereWidget(i);
        if(m_displayColorMapByDirection[i] || m_displayVectorsByDirection[i])
        {
            this->creationSphereWidget(i);
        }

        m_windowsList[i]->Render();
    }
}

vtkRenderWindow* ShapePopulationBase::CreateNewWindow(std::string a_filePath, bool testing)
{
    //DATA
    ShapePopulationData * a_mesh = new ShapePopulationData;
    a_mesh->ReadMesh(a_filePath);
    return CreateNewWindow(a_mesh, testing);
}

vtkRenderWindow* ShapePopulationBase::CreateNewWindow(vtkPolyData* a_popyData, std::string a_filePath, bool testing)
{
    //DATA
    ShapePopulationData * a_mesh = new ShapePopulationData;
    a_mesh->ReadMesh(a_popyData, a_filePath);
    return CreateNewWindow(a_mesh, testing);
}

vtkRenderWindow* ShapePopulationBase::CreateNewWindow(ShapePopulationData* a_mesh, bool testing)
{
    ShapePopulationData* Mesh = a_mesh;
    m_meshList.push_back(Mesh);

    //MAPPER
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(Mesh->GetPolyData());

    //ACTOR
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    /* VECTORS */

    //Arrow

    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
    glyph->SetSourceConnection(arrow->GetOutputPort());
    glyph->SetInputData(Mesh->GetPolyData());
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
    glyphMapper->SetInputData(glyph->GetOutput());
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
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    if (!testing)
      {
      renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
      }
    else
      {
      renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
      }
    renderWindow->AddRenderer(renderer);
    //renderWindow->SetAlphaBitPlanes(true);/*test opacity*/
    //renderWindow->SetMultiSamples(0);/*test opacity*/
    m_windowsList.push_back(renderWindow);

    //INTERACTOR
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindow->SetInteractor(interactor);

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
    attributeName->SetLinearFontScaleFactor(2);
    attributeName->SetNonlinearFontScaleFactor(1);
    attributeName->SetMaximumFontSize(15);
    attributeName->SetText(0," ");
    attributeName->GetTextProperty()->SetColor(m_labelColor);
    renderer->AddViewProp(attributeName);

    // SCALAR BAR
    vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
    scalarBar->SetLookupTable(mapper->GetLookupTable());
//    scalarBar->SetTitle("Title");
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

    return renderWindow;
}

void ShapePopulationBase::UpdateWindows()
{
    unsigned int i;
    for (i = 0; i < m_meshList.size(); i++)
    {
        //MAPPER
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(m_meshList[i]->GetPolyData());

        //ACTOR
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);

        /* VECTORS */
        //Arrow
        vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
        m_glyphList[i]->SetSourceConnection(arrow->GetOutputPort());
        m_glyphList[i]->SetInputData(m_meshList[i]->GetPolyData());
        m_glyphList[i]->ScalingOn();
        m_glyphList[i]->OrientOn();
        m_glyphList[i]->ClampingOff();
        m_glyphList[i]->SetColorModeToColorByVector();
        m_glyphList[i]->SetScaleModeToScaleByVector();
        m_glyphList[i]->SetVectorModeToUseVector();
        m_glyphList[i]->Update();

        //Mapper & Actor
        vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        glyphMapper->SetInputData(m_glyphList[i]->GetOutput());
        vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
        glyphActor->SetMapper(glyphMapper);

        /* END VECTORS */

        //RENDERER
        auto renderer = m_windowsList[i]->GetRenderers()->GetFirstRenderer();
        renderer->RemoveAllViewProps();

        renderer->AddActor(actor);
        renderer->AddActor(glyphActor);
        renderer->SetActiveCamera(m_headcam); //set the active camera for this renderer to main camera
        renderer->ResetCamera();
        //renderer->SetUseDepthPeeling(true);/*test opacity*/

//        //INTERACTOR
//        vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//        renderWindow->SetInteractor(interactor);

        //ANNOTATIONS (file name)
        vtkSmartPointer<vtkCornerAnnotation> fileName = vtkSmartPointer<vtkCornerAnnotation>::New();
        fileName->SetLinearFontScaleFactor( 2 );
        fileName->SetNonlinearFontScaleFactor( 1 );
        fileName->SetMaximumFontSize( 15 );
        fileName->SetText(2,m_meshList[i]->GetFileName().c_str());
        fileName->GetTextProperty()->SetColor(m_labelColor);
        renderer->AddViewProp(fileName);

        //ANNOTATIONS (attribute name)
        vtkSmartPointer<vtkCornerAnnotation> attributeName = vtkSmartPointer<vtkCornerAnnotation>::New();
        attributeName->SetLinearFontScaleFactor(2);
        attributeName->SetNonlinearFontScaleFactor(1);
        attributeName->SetMaximumFontSize(15);
        attributeName->SetText(0," ");
        attributeName->GetTextProperty()->SetColor(m_labelColor);
        renderer->AddViewProp(attributeName);

        // SCALAR BAR
        vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        scalarBar->SetLookupTable(mapper->GetLookupTable());
    //    scalarBar->SetTitle("Title");
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
    RenderAll();
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
    vtkRenderWindowInteractor* selectedInteractor = vtkRenderWindowInteractor::SafeDownCast(a_selectedObject);
    vtkRenderWindow* selectedWindow = selectedInteractor->GetRenderWindow();
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
        int dim = m_meshList[m_selectedIndex[0]]->GetPolyData()->GetPointData()->GetScalars(cmap)->GetNumberOfComponents();
        if(dim == 1) this->UpdateColorMapByMagnitude(m_selectedIndex);
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
    vtkRenderWindowInteractor* selectedInteractor = vtkRenderWindowInteractor::SafeDownCast(a_selectedObject);

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

    bool saved_renderAllSelection = m_renderAllSelection;
    m_renderAllSelection = false;
    for (unsigned int i = 0; i < m_selectedIndex.size();i++) //render all windows selected (one of them will be the event window)
    {
        m_windowsList[m_selectedIndex[i]]->Render();
    }
    m_renderAllSelection = saved_renderAllSelection;

    //this->sendCameraConfig();
}

void ShapePopulationBase::RealTimeRenderSynchro(bool realtime)
{
    // Remove observers
    for (size_t idx = 0; idx < m_windowsList.size() && idx < m_windowsListObserverTags.size(); ++idx)
      {
      vtkRenderWindow* renderWindow = m_windowsList[idx];
      renderWindow->RemoveObserver(renderWindow->GetCommand(m_windowsListObserverTags[idx]));
      }

    m_windowsListObserverTags.clear();

    // Synchronize when rendering
    for (unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        m_windowsListObserverTags.push_back(
              m_windowsList[i]->AddObserver(
                realtime ? vtkCommand::RenderEvent : vtkCommand::ModifiedEvent,
                this, &ShapePopulationBase::RenderSelection));
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
        size_t numScalars = m_meshList.at(i)->GetAttributeList().size();
        if(i==0)
        {
            for (size_t j = 0; j < numScalars; j++)
            {
                std::string scalarName = m_meshList.at(i)->GetAttributeList().at(j);
                m_commonAttributes.push_back(scalarName);
            }
        }
        else
        {
            std::vector<std::string> Attributes2;
            for (size_t j = 0; j < numScalars; j++)
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


        vtkPolyData* polyData = mesh->GetPolyData();
//        vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();

//        normalGenerator->SetInputData(polyData);
//        normalGenerator->Update();

//        polyData = normalGenerator->GetOutput();

        vtkDataArray* normalDataDouble = polyData->GetPointData()->GetArray(cmap);

        magnitudStruct* magnitude = m_magnitude[index];
        for(int l = 0; l  < numPts; l++)
        {
            double normal[3];
            normalDataDouble->GetTuple(l, normal);

            //NORMALIZE THE VECTOR, CALCULATE THE COEFFICIENT
            float magcoeff = vtkMath::Normalize(normal)/(magnitude->max);

            float RGB[3];
            float r = 0;
            float g = 0;
            float b = 0;

            // Color the minus axis with the complementary
            if(m_axisColor[m_selectedIndex[i]]->complementaryColor)
            {
                r = (1.0 + normal[0])/2.0;
                g = (1.0 + normal[1])/2.0;
                b = (1.0 + normal[2])/2.0;
            }

            // Color the minus axis with with the same color
            else if(m_axisColor[m_selectedIndex[i]]->sameColor)
            {
                r = (1.0 + fabs(normal[0]))/2.0;
                g = (1.0 + fabs(normal[1]))/2.0;
                b = (1.0 + fabs(normal[2]))/2.0;
            }

            //CALCULATE THE WEIGHTS FOR THE COLORING
            float fn[3];
            fn[0] = fabs(normal[0]);
            fn[1] = fabs(normal[1]);
            fn[2] = fabs(normal[2]);
            float w = fn[0] + fn[1] + fn[2];

            // COMPUTE THE COEF TO HAVE THE COLOR CHOOSE BY THE USER
            for( int k = 0; k < 3; k++)
            {
                double Xcoef[3];
                double Ycoef[3];
                double Zcoef[3];
                Xcoef[k] = (m_axisColor[m_selectedIndex[i]]->XAxiscolor[k])/255;
                Ycoef[k] = (m_axisColor[m_selectedIndex[i]]->YAxiscolor[k])/255;
                Zcoef[k] = (m_axisColor[m_selectedIndex[i]]->ZAxiscolor[k])/255;

                //USE THE WEIGHTS AND COMPUTE THE CONTRIBUTION FOR EACH COMPONENT RGB.

                //[ 1-r  1-g  1-b        [fn[0]
                //  1-r  1-g  1-b    X    fn[1]   X  255/(w*magcoeff)
                //  1-r  1-g  1-b ]       fn[2]]
                RGB[k] = (fabs(1 - r - Xcoef[k])*fn[0] + fabs(1 - g - Ycoef[k])*fn[1] + fabs(1 - b - Zcoef[k])*fn[2])*255/w*magcoeff;

                if( RGB[k] > 255 ) RGB[k] = 255;
                if( RGB[k] < 0 ) RGB[k] = 0;
            }
            scalars->InsertTuple3(l, (unsigned char)RGB[0], (unsigned char)RGB[1], (unsigned char)RGB[2]);

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
    if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()))
    {
        dim = 3;
        a_cmap = new_cmap.c_str();
    }

    /* UPDATE ATTRIBUTE NAME (cornerAnnotation) */
    for (unsigned int i = 0; i < a_windowIndex.size(); i++)
    {
        vtkPropCollection* propCollection =  m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();

        //CornerAnnotation Update
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);
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
            vtkActor* glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();

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

        for (unsigned int i = 0; i < a_windowIndex.size(); i++)
        {
            strs_mag.str(""); strs_mag.clear();
            strs_mag << a_cmap << "_mag" << std::endl;
            strs_dir.str(""); strs_dir.clear();
            strs_dir << a_cmap << "_ColorByDirection" << std::endl;


            ShapePopulationData * mesh = m_meshList[a_windowIndex[i]];
            vtkRenderWindow * window = m_windowsList[a_windowIndex[i]];
            vtkActor* glyphActor = window->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();

            // Set Active Vectors
            mesh->GetPolyData()->GetPointData()->SetActiveVectors(a_cmap);

            // Set Active scalars
            // display colormap by direction
            if(m_displayColorMapByDirection[a_windowIndex[i]])
            {
                mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_dir.str().c_str());
            }

            // display colormap by magnitude
            else
            {
                mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_mag.str().c_str());
            }


            // Update Glyph
            vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
            vtkGlyph3D* glyph = m_glyphList[a_windowIndex[i]];
            glyph->SetSourceConnection(arrow->GetOutputPort());
            glyph->Update();

            // Vectors
            if(!m_noUpdateVectorsByDirection)
            {
                if(m_displayVectorsByDirection[a_windowIndex[i]])
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
        vtkPropCollection* propCollection =  m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
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
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << cmap;

            // Set Active Scalars
            mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs.str().c_str());

            // Hide or show the scalar bar
            vtkPropCollection* propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
            if(m_displayColorbar)
            {
                scalarBar->SetVisibility(1);
            }

            // Hide or show sphere and title of this widget
            if(m_displayVectorsByDirection[m_selectedIndex[i]])
            {
                this->creationSphereWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
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

            // Set Active Scalars for the ColorMap
            mesh->GetPolyData()->GetPointData()->SetActiveScalars(cmap.c_str());

            // Hide or show the scalar bar
            vtkPropCollection* propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
            if(!m_displayVectorsByMagnitude[m_selectedIndex[i]])
            {
                scalarBar->SetVisibility(0);
            }

            // Hide or show sphere
            this->creationSphereWidget(m_selectedIndex[i]);
        }
    }
}

void ShapePopulationBase::UpdateColorMapByMagnitude(std::vector< unsigned int > a_windowIndex)
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
        vtkMapper* mapper = actors->GetNextActor()->GetMapper();
        mapper->SetLookupTable( DistanceMapTFunc );
        mapper->ScalarVisibilityOn();

        //Vector Mapper Update
        vtkMapper* glyphMapper = actors->GetLastActor()->GetMapper();
        glyphMapper->SetLookupTable( DistanceMapTFunc );


        //ScalarBar Mapper Update
        vtkPropCollection* propCollection =  m_windowsList[a_windowIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        scalarBar->SetLookupTable( DistanceMapTFunc );
//        scalarBar->SetTitle(" ");
    }
}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            VECTORS                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::setMeshOpacity(double value)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        m_meshOpacity[m_selectedIndex[i]] = (int)(value*100);
        vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
        actors->InitTraversal();
        actors->GetNextActor()->GetProperty()->SetOpacity(value);
    }
}

void ShapePopulationBase::setVectorScale(double value)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        m_vectorScale[m_selectedIndex[i]] = (int)(value*100);

        //        vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
        vtkGlyph3D* glyph = m_glyphList[m_selectedIndex[i]];
        //        glyph->SetSourceConnection(arrow->GetOutputPort());
        glyph->SetScaleFactor(value);
        glyph->Update();
//        if(!m_noUpdateVectorsByDirection)
//        {
            if(m_displayVectorsByDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();
//        }
    }
}

void ShapePopulationBase::setVectorDensity(double value)
{
    for(unsigned int i = 0; i < m_selectedIndex.size() ; i++)
    {
        m_vectorDensity[m_selectedIndex[i]] = value;

        ShapePopulationData * mesh = m_meshList[m_selectedIndex[i]];
        vtkSmartPointer<vtkDecimatePro> filter = vtkSmartPointer<vtkDecimatePro>::New();
        filter->SetInputData(mesh->GetPolyData());
        filter->SetTargetReduction(1 - value/100);

        vtkGlyph3D* glyph = m_glyphList[m_selectedIndex[i]];
        glyph->SetInputConnection(filter->GetOutputPort());
        glyph->Update();
//        if(!m_noUpdateVectorsByDirection)
//        {
            if(m_displayVectorsByDirection[m_selectedIndex[i]]) this->UpdateVectorsByDirection();
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

        std::string key2 ("_ColorByDirection");
        found = cmap.rfind(key2);
        new_cmap = new_cmap.substr(0,found);

        if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()))
        {
            vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
            vtkActor* glyphActor = actors->GetLastActor();
            if(display) glyphActor->SetVisibility(1);
            else glyphActor->SetVisibility(0);
        }

        // Hide or show the scalar bar
        vtkPropCollection* propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
        if(!m_displayVectors[m_selectedIndex[i]])
        {
            if(m_displayColorMapByDirection[m_selectedIndex[i]]) scalarBar->SetVisibility(0);
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
            }
        }
        else // delete of vectors
        {
            // Hide sphere
            if(!m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);

            }

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

            if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()) )
            {
                vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
                vtkGlyph3D* glyph = m_glyphList[m_selectedIndex[i]];
                glyph->SetSourceConnection(arrow->GetOutputPort());
                glyph->SetColorModeToColorByVector();
                glyph->Update();

                vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
                vtkActor* glyphActor = actors->GetLastActor();

                if(display) glyphActor->SetVisibility(1);
                else glyphActor->SetVisibility(0);
            }
            // Hide or show the scalar bar
            vtkPropCollection* propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
            if(m_displayColorbar) scalarBar->SetVisibility(1);

            // Hide or show sphere and title of this widget
            if(m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                this->creationSphereWidget(m_selectedIndex[i]);
            }
            else
            {
                if(!m_widgetSphere.empty()) this->deleteSphereWidget(m_selectedIndex[i]);
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
            std::ostringstream strs;
            strs.str(""); strs.clear();
            strs << new_cmap;

            if( (new_cmap != cmap) && (std::find(m_commonAttributes.begin(), m_commonAttributes.end(), new_cmap) != m_commonAttributes.end()) )
            {

                if(m_displayColorMapByDirection[m_selectedIndex[i]])
                {
                    mesh->GetPolyData()->GetPointData()->SetActiveVectors(strs.str().c_str());

                    vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
                    vtkGlyph3D* glyph = m_glyphList[m_selectedIndex[i]];
                    glyph->SetSourceConnection(arrow->GetOutputPort());
                    glyph->SetColorModeToColorByScalar();
                    glyph->Update();
                }
                else if (m_displayColorMapByMagnitude[m_selectedIndex[i]]) this->UpdateVectorsByDirection();

                vtkActorCollection * actors = m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetActors();
                vtkActor* glyphActor = actors->GetLastActor();
                if(display) glyphActor->SetVisibility(1);
                else glyphActor->SetVisibility(0);
            }

            // Hide or show the scalar bar
            vtkPropCollection* propCollection =  m_windowsList[m_selectedIndex[i]]->GetRenderers()->GetFirstRenderer()->GetViewProps();
            vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
            vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);
            if(m_displayColorMapByDirection[m_selectedIndex[i]])
            {
                scalarBar->SetVisibility(0);
            }
            else if (m_displayColorMapByMagnitude[m_selectedIndex[i]] && m_displayColorbar)
            {
                scalarBar->SetVisibility(1);
            }

            // Show sphere
            this->creationSphereWidget(m_selectedIndex[i]);
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

        std::ostringstream strs;
        strs.str(""); strs.clear();
        strs << new_cmap;
        std::ostringstream strs_mag;
        strs_mag.str(""); strs_mag.clear();
        strs_mag << new_cmap << "_mag" << std::endl;
        std::ostringstream strs_dir;
        strs_dir.str(""); strs_dir.clear();
        strs_dir << new_cmap << "_ColorByDirection" << std::endl;

        // Set Active Scalars to color vectors
        if(m_displayVectorsByDirection[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_dir.str().c_str());

        // Set Active Vectors
        mesh->GetPolyData()->GetPointData()->SetActiveVectors(strs.str().c_str());

        vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
        vtkGlyph3D* glyph = m_glyphList[m_selectedIndex[i]];
        glyph->SetSourceConnection(arrow->GetOutputPort());
        glyph->SetColorModeToColorByScalar();
        glyph->Update();

        // Set Active Scalars to re-color the colormap
        if(m_displayColorMapByMagnitude[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_mag.str().c_str());
        else if (m_displayColorMapByDirection[m_selectedIndex[i]]) mesh->GetPolyData()->GetPointData()->SetActiveScalars(strs_dir.str().c_str());
    }

}


// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            DISPLAY                                            * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //

void ShapePopulationBase::displayColorbar(bool display)
{
    for(unsigned int i = 0; i < m_windowsList.size() ; i++)
    {
        vtkPropCollection* propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();

        // scalar bar
        vtkObject * viewPropObject = propCollection->GetItemAsObject(4);
        vtkScalarBarActor* scalarBar = vtkScalarBarActor::SafeDownCast(viewPropObject);

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
        vtkPropCollection* propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();

        // cornerAnnotation
        vtkObject * viewPropObject = propCollection->GetItemAsObject(3);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);

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
        vtkPropCollection* propCollection =  m_windowsList[i]->GetRenderers()->GetFirstRenderer()->GetViewProps();

        // cornerAnnotation
        vtkObject * viewPropObject = propCollection->GetItemAsObject(2);
        vtkCornerAnnotation* cornerAnnotation = vtkCornerAnnotation::SafeDownCast(viewPropObject);

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
            }
        }
        else
        {
            m_displaySphere = false;
            if(!m_widgetSphere.empty()) deleteSphereWidget(i);
        }
    }
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

//    vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();

//    normalGenerator->SetInputData(polyData);
//    normalGenerator->Update();

//    polyData = normalGenerator->GetOutput();

    vtkDataArray* normalDataDouble = polyData->GetPointData()->GetArray("Normals");

    // Color map by direction
    vtkIdType numPts = polyData->GetNumberOfPoints();
    vtkSmartPointer<vtkUnsignedCharArray> scalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
    scalars->SetNumberOfComponents(3);
    scalars->SetNumberOfTuples(numPts);
    scalars->SetName("ColorByDirection");

    double magnitude = -1;
    for(int i = 0; i  < numPts; i++)
    {
        double normal[3];
        normalDataDouble->GetTuple(i, normal);
        magnitude = std::max(magnitude, vtkMath::Normalize(normal));
    }

    for(int l = 0; l  < numPts; l++)
    {
        double normal[3];
        normalDataDouble->GetTuple(l, normal);

        //NORMALIZE THE VECTOR, CALCULATE THE COEFFICIENT
        float magcoeff = vtkMath::Normalize(normal)/(magnitude);

        float RGB[3];
        float r = 0;
        float g = 0;
        float b = 0;

        // Color the minus axis with the complementary
        if(m_axisColor[m_selectedIndex[0]]->complementaryColor)
        {
            r = (1.0 + normal[0])/2.0;
            g = (1.0 + normal[1])/2.0;
            b = (1.0 + normal[2])/2.0;
        }

        // Color the minus axis with with the same color
        else if(m_axisColor[m_selectedIndex[0]]->sameColor)
        {
            r = (1.0 + fabs(normal[0]))/2.0;
            g = (1.0 + fabs(normal[1]))/2.0;
            b = (1.0 + fabs(normal[2]))/2.0;
        }

        //CALCULATE THE WEIGHTS FOR THE COLORING
        float fn[3];
        fn[0] = fabs(normal[0]);
        fn[1] = fabs(normal[1]);
        fn[2] = fabs(normal[2]);
        float w = fn[0] + fn[1] + fn[2];

        // COMPUTE THE COEF TO HAVE THE COLOR CHOOSE BY THE USER
        axisColorStruct* axisColor = m_axisColor[m_selectedIndex[0]];
        for( int k = 0; k < 3; k++)
        {
            double Xcoef[3];
            double Ycoef[3];
            double Zcoef[3];
            Xcoef[k] = (axisColor->XAxiscolor[k])/255;
            Ycoef[k] = (axisColor->YAxiscolor[k])/255;
            Zcoef[k] = (axisColor->ZAxiscolor[k])/255;

            //USE THE WEIGHTS AND COMPUTE THE CONTRIBUTION FOR EACH COMPONENT RGB.

            //[ 1-r  1-g  1-b        [fn[0]
            //  1-r  1-g  1-b    X    fn[1]   X  255/(w*magcoeff)
            //  1-r  1-g  1-b ]       fn[2]]
            RGB[k] = (fabs(1 - r - Xcoef[k])*fn[0] + fabs(1 - g - Ycoef[k])*fn[1] + fabs(1 - b - Zcoef[k])*fn[2])*255/w*magcoeff;

            if( RGB[k] > 255 ) RGB[k] = 255;
            if( RGB[k] < 0 ) RGB[k] = 0;
        }
        scalars->InsertTuple3(l, (unsigned char)RGB[0], (unsigned char)RGB[1], (unsigned char)RGB[2]);
    }
    polyData->GetPointData()->SetScalars(scalars);

    // Map of the downloaded figure
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);

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

        vtkOrientationMarkerWidget* widgetSphere = m_widgetSphere[index];
//        widgetSphere->SetOutlineColor( 1, 1, 1 ); // color for the frame around the axes
        widgetSphere->SetOrientationMarker( actorSphere );
        widgetSphere->SetInteractor( iren );
        widgetSphere->SetViewport( 0.0, 0.40, 0.15, 0.65 ); // size and position of the frame
        widgetSphere->SetEnabled( 1 );
        widgetSphere->InteractiveOff();


//        // AXIS
        vtkSmartPointer<vtkAxesActor> actorAxisByDirection = vtkSmartPointer<vtkAxesActor>::New();
        actorAxisByDirection->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(m_labelColor);
        actorAxisByDirection->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(m_labelColor);
        actorAxisByDirection->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(m_labelColor);

        // color of axis
        axisColorStruct* axisColor = m_axisColor[index];
        double Xcoef[3];
        double Ycoef[3];
        double Zcoef[3];
        for( int k = 0; k < 3; k++)
        {
            Xcoef[k] = (axisColor->XAxiscolor[k])/255;
            Ycoef[k] = (axisColor->YAxiscolor[k])/255;
            Zcoef[k] = (axisColor->ZAxiscolor[k])/255;
        }
        actorAxisByDirection->GetXAxisShaftProperty()->SetColor(Xcoef[0],Xcoef[1],Xcoef[2]);
        actorAxisByDirection->GetXAxisTipProperty()->SetColor(Xcoef[0],Xcoef[1],Xcoef[2]);
        actorAxisByDirection->GetYAxisShaftProperty()->SetColor(Ycoef[0],Ycoef[1],Ycoef[2]);
        actorAxisByDirection->GetYAxisTipProperty()->SetColor(Ycoef[0],Ycoef[1],Ycoef[2]);
        actorAxisByDirection->GetZAxisShaftProperty()->SetColor(Zcoef[0],Zcoef[1],Zcoef[2]);
        actorAxisByDirection->GetZAxisTipProperty()->SetColor(Zcoef[0],Zcoef[1],Zcoef[2]);

        vtkOrientationMarkerWidget* widgetAxisByDirection = m_widgetAxisByDirection[index];
//        widgetAxisByDirection->SetOutlineColor( 1, 1, 1 ); // color for the frame around the axes
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
    if(index < static_cast<int>(m_widgetSphere.size()))
    {
        if (m_createSphere[index])
        {
            if (m_widgetSphere[index].GetPointer())
            {
                m_widgetSphere[index]->SetEnabled( 0 );
            }
            m_widgetSphere[index] = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

            if (m_widgetAxisByDirection[index].GetPointer())
            {
                m_widgetAxisByDirection[index]->SetEnabled( 0 );
            }
            m_widgetAxisByDirection[index] = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
        }
        m_createSphere[index] = false;
    }
}


void ShapePopulationBase::deleteAllWidgets()
{
    for (unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        if(!m_widgetSphere.empty())deleteSphereWidget(i);
    }
}

void ShapePopulationBase::initializationAllWidgets()
{
    m_widgetSphere.clear();
    m_widgetAxisByDirection.clear();
    m_createSphere.clear();

    // initialization of all the widgets
    for (unsigned int i = 0; i < m_windowsList.size(); i++)
    {
        m_widgetSphere.push_back(vtkSmartPointer<vtkOrientationMarkerWidget>::New());
        m_widgetAxisByDirection.push_back(vtkSmartPointer<vtkOrientationMarkerWidget>::New());
        m_createSphere.push_back(false);

    }
}

// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
// *                                            CAMERA                                             * //
// * ///////////////////////////////////////////////////////////////////////////////////////////// * //
void ShapePopulationBase::ChangeView(int R, int A, int S,int x_ViewUp,int y_ViewUp,int z_ViewUp)
{
    vtkRenderer* firstRenderer = m_windowsList[m_selectedIndex[0]]->GetRenderers()->GetFirstRenderer();
    firstRenderer->ResetCamera();

    double *coords  = firstRenderer->GetActiveCamera()->GetFocalPoint();
    double distance = firstRenderer->GetActiveCamera()->GetDistance();
    firstRenderer->GetActiveCamera()->SetPosition(coords[0]+R*distance,coords[1]+A*distance,coords[2]+S*distance);

    //setroll to .001, because it breaks on y axis if roll = 0
    firstRenderer->GetActiveCamera()->SetRoll(.001);

    // View Up
    firstRenderer->GetActiveCamera()->SetViewUp(x_ViewUp,y_ViewUp,z_ViewUp);

    m_renderAllSelection = true;
    this->RenderSelection();
    m_renderAllSelection = false;
    this->UpdateCameraConfig();

}

void ShapePopulationBase::ResetHeadcam()
{

    if(m_selectedIndex.empty())
    {
        vtkRenderer* firstRenderer = m_windowsList[0]->GetRenderers()->GetFirstRenderer();
        firstRenderer->ResetCamera();
        m_headcam->DeepCopy(firstRenderer->GetActiveCamera());
    }
    vtkRenderer* firstRenderer = m_windowsList[m_selectedIndex[0]]->GetRenderers()->GetFirstRenderer();
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
            vtkActor* meshActor = actors->GetNextActor();
            vtkActor* glyphActor = actors->GetLastActor();
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
            vtkActor* meshActor = actors->GetNextActor();
            vtkActor* glyphActor = actors->GetLastActor();

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

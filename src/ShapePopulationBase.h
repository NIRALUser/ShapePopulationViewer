#ifndef SHAPEPOPULATIONBASE_H
#define SHAPEPOPULATIONBASE_H

#include <vtkVersion.h>

#include "ShapePopulationData.h"
#include "ShapePopulationStruct.h"
#include "ShapePopulationViewerConfig.h"

#include <vtkCamera.h>                      //Camera
#include <vtkPolyDataMapper.h>              //Mapper
#include <vtkActor.h>                       //Actor
#include <vtkProperty.h>                    //Actor Opacity
#include <vtkRenderer.h>                    //Renderer
#include <vtkRenderWindow.h>                //RenderWindow
#include <vtkRenderWindowInteractor.h>      //Interactor
#include <vtkCornerAnnotation.h>            //Window Annotations
#include <vtkTextProperty.h>                //Annotations Text
#include <vtkScalarBarActor.h>              //ScalarBar
#include <vtkColorTransferFunction.h>       //LookUpTable
#include <vtkCommand.h>                     //Event
#include <vtkRendererCollection.h>          //GetRenderers
#include <vtkActor2DCollection.h>           //GetActors2D
#include <vtkAxesActor.h>                   //Axes Actor
#include <vtkTextActor.h>                   //Text Actor
#include <vtkOrientationMarkerWidget.h>     //Widgets
#include <vtkSphereSource.h>                //Sphere
#include <vtkCaptionActor2D.h>              //Caption's Proprety of Axis
#include <string.h>
#include <vtkMath.h>
#include <vtkDoubleArray.h>

#include "vtkGlyph3D.h"
#include "vtkArrowSource.h"
#include "vtkDecimatePro.h"

#include <set>

class ShapePopulationBase
{
    friend class TestShapePopulationBase;

    public :

    ShapePopulationBase();
    ~ShapePopulationBase();


    void KeyPressEventVTK(vtkObject* a_selectedObject, unsigned long, void*);
    void CameraChangedEventVTK(vtkObject*, unsigned long, void*);
    void StartEventVTK(vtkObject*, unsigned long, void*);
    void EndEventVTK(vtkObject*, unsigned long, void*);

    protected :

    std::vector<ShapePopulationData *> m_meshList;
    std::vector< vtkSmartPointer<vtkGlyph3D> > m_glyphList;
    std::vector< vtkSmartPointer<vtkRenderWindow> > m_windowsList;
    std::vector< unsigned long > m_windowsListObserverTags;
    std::vector< unsigned int > m_selectedIndex;
    vtkSmartPointer<vtkCamera> m_headcam;
    cameraConfigStruct m_headcamConfig;
    std::vector<std::string> m_commonAttributes;
    colorBarStruct * m_usedColorBar;
    std::vector< colorBarStruct *> m_colorBarList;
    bool m_renderAllSelection;
    std::vector<bool> m_displayColorMapByMagnitude;
    std::vector<bool> m_displayColorMapByDirection;
    std::vector<bool> m_displayVectors;
    std::vector<bool> m_displayVectorsByMagnitude;
    std::vector<bool> m_displayVectorsByDirection;
    std::vector<int> m_meshOpacity;
    std::vector<int> m_vectorScale;
    std::vector<int> m_vectorDensity;
    bool m_displayColorbar;
    bool m_displayAttribute;
    bool m_displayMeshName;
    bool m_displaySphere;
    bool m_noUpdateVectorsByDirection;
    std::vector<bool> m_createSphere;
    std::vector< vtkSmartPointer<vtkOrientationMarkerWidget> > m_widgetSphere;
    std::vector< vtkSmartPointer<vtkOrientationMarkerWidget> > m_widgetAxisByDirection;
    std::vector< magnitudStruct * > m_magnitude;
    magnitudStruct* m_usedMagnitude;
    std::vector< axisColorStruct* > m_axisColor;

    // Setting \a testing to true means a \a vtkRenderWindow is created
    // instead of \a vtkGenericOpenGLRenderWindow.
    vtkRenderWindow* CreateNewWindow(std::string a_filePath, bool testing = false);
    vtkRenderWindow* CreateNewWindow(vtkPolyData* a_popyData, std::string a_filePath, bool testing = false);
    vtkRenderWindow* CreateNewWindow(ShapePopulationData* a_mesh, bool testing = false);
    void UpdateWindows();

    //SELECTION
    unsigned int getSelectedIndex(vtkSmartPointer<vtkRenderWindow> a_selectedWindow);
    virtual void ClickEvent(vtkObject* a_selectedObject, unsigned long, void*);
    virtual void SelectAll();
    virtual void UnselectAll();

    //RENDERING
    void RenderAll();
    void RenderSelection();
    void RealTimeRenderSynchro(bool realtime);

    //COLORMAP
    double m_commonRange[2];
    double m_commonMagnitud[2];
    void computeCommonAttributes();
    double* computeCommonRange(const char * a_cmap, std::vector<unsigned int> a_windowIndex);
    void UpdateColorMapByDirection(const char *cmap, int index);
    void UpdateAttribute(const char *a_cmap, std::vector<unsigned int> a_windowIndex);
    void displayColorMapByMagnitude(bool display);
    void displayColorMapByDirection(bool display);
    void UpdateColorMapByMagnitude(std::vector<unsigned int> a_windowIndex);

    //VECTORS
    void setMeshOpacity(double value);
    void setVectorScale(double value);
    void setVectorDensity(double value);
    void displayVectors(bool display);
    void displayVectorsByMagnitude(bool display);
    void displayVectorsByDirection(bool display);
    void UpdateVectorsByDirection();

    //DISPLAY
    void displayColorbar(bool display);
    void displayAttribute(bool display);
    void displayMeshName(bool display);
    void displaySphere(bool display);

    // AXIS WIDGETS and SPHERE WIDGETS
    vtkActor* creationSphereActor();
    void creationSphereWidget(int index);
    void deleteSphereWidget(int index);
        // Delete axis and sphere widgets
    void deleteAllWidgets();
        // Initialization of all the widgets
    void initializationAllWidgets();

    //CAMERA/VIEW
    void AlignMesh(bool alignment);
    void ChangeView(int R, int A, int S,int x_ViewUp,int y_ViewUp,int z_ViewUp);
    void ResetHeadcam();
    virtual void UpdateCameraConfig();

    //BACKGROUND
    double m_selectedColor[3];
    double m_unselectedColor[3];
    double m_labelColor[3];
    void setBackgroundSelectedColor(double a_selectedColor[]);
    void setBackgroundUnselectedColor(double a_unselectedColor[]);
    void setLabelColor(double a_labelColor[]);

};


#endif

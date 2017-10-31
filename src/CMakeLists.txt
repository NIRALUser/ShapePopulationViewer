# --- cmake -------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.3)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

# --- INCLUDES ----------------------------------------------------------------------------
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

if( VTK_VERSION_MAJOR EQUAL 6 )
  set(Slicer_VTK_COMPONENTS
  vtkChartsCore
  vtkCommonColor
  vtkCommonComputationalGeometry
  vtkCommonCore
  vtkCommonDataModel
  vtkCommonExecutionModel
  vtkCommonMath
  vtkCommonMisc
  vtkCommonSystem
  vtkCommonTransforms
  vtkFiltersCore
  vtkFiltersExtraction
  vtkFiltersGeneral
  vtkFiltersHybrid
  vtkFiltersImaging
  vtkFiltersModeling
  vtkFiltersSources
  vtkFiltersStatistics
  vtkGUISupportQt
  vtkIOCore
  vtkIOGeometry
  vtkIOImage
  vtkIOInfovis
  vtkIOLegacy
  vtkIOXML
  vtkIOXMLParser
  vtkImagingColor
  vtkImagingCore
  vtkImagingFourier
  vtkImagingGeneral
  vtkImagingHybrid
  vtkImagingSources
  vtkInfovisCore
  vtkInfovisLayout
  vtkInteractionStyle
  vtkInteractionWidgets
  vtkRenderingAnnotation
  vtkRenderingContext2D
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingLabel
  vtkRenderingOpenGL
  vtkRenderingVolume
  vtkViewsCore
  vtkViewsInfovis
  vtkViewsQt
  vtkalglib
  vtkfreetype
  vtkftgl
  vtkzlib
  )
  if( NOT ShapePopulationViewer_BUILD_SLICER_EXTENSION )
    list( APPEND Slicer_VTK_COMPONENTS vtkIOExport )
  endif()
endif()


find_package(VTK REQUIRED )
include(${VTK_USE_FILE})
string(FIND "${VTK_MODULES_ENABLED}" "GL2PS" index)

if( ${index} EQUAL -1)
  add_definitions("-DSPV_EXTENSION=1")
endif()

# --- SRCS --------------------------------------------------------------------------------
file(GLOB CXX_FILES *.cxx)
file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *QT.h)
file(GLOB QT_QRC_FILES *.qrc)
QT4_WRAP_UI(UISrcs ${UI_FILES})
QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
QT4_ADD_RESOURCES(QRCrcs ${QT_QRC_FILES})

# --- LIBRARIES ---------------------------------------------------------------------------
if(VTK_LIBRARIES)
  if(${VTK_VERSION} VERSION_LESS "6")
    set(ShapePopulationLIBS ${VTK_LIBRARIES} QVTK)
  else()
    set(ShapePopulationLIBS ${VTK_LIBRARIES})
  endif()
else()
  set(ShapePopulationLIBS vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
endif()

# --- SEMMacro ----------------------------------------------------------------------------
SEMMacroBuildCLI(
  NAME ${LOCAL_PROJECT_NAME}
  EXECUTABLE_ONLY
  ADDITIONAL_SRCS
    ${CXX_FILES}
    ${UISrcs}
    ${MOCSrcs}
    ${QRCrcs}
  TARGET_LIBRARIES ${ShapePopulationLIBS}
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )


# --- CTest -------------------------------------------------------------------------------
#set(BUILD_TESTING ON CACHE BOOL "Build, configure and copy testing files")

if( BUILD_TESTING )
  ADD_SUBDIRECTORY(Testing)
endif()



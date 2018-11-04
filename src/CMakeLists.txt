
# --- INCLUDES ----------------------------------------------------------------------------
string(FIND "${VTK_MODULES_ENABLED}" "GL2PS" index)
if( ${index} EQUAL -1)
  add_definitions("-DSPV_EXTENSION=1")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ShapePopulationBase.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/ShapePopulationBase.h
  )

# --- SRCS --------------------------------------------------------------------------------
set(CXX_FILES
  backgroundDialogQT.cxx
  cameraDialogQT.cxx
  CSVloaderQT.cxx
  customizeColorMapByDirectionDialogQT.cxx
  gradientArrow.cxx
  gradientWidgetQT.cxx
  ShapePopulationBase.cxx
  ShapePopulationData.cxx
  ShapePopulationMainWindowQT.cxx
  ShapePopulationQT.cxx
  ShapePopulationViewer.cxx
  vtkPVPostFilter.cxx
  vtkPVPostFilterExecutive.cxx
  )

set(UI_FILES
  backgroundDialogQT.ui
  cameraDialogQT.ui
  CSVloaderQT.ui
  customizeColorMapByDirectionDialogQT.ui
  ShapePopulationMainWindowQT.ui
  ShapePopulationQT.ui
  )

set(QT_WRAP
  backgroundDialogQT.h
  cameraDialogQT.h
  CSVloaderQT.h
  customizeColorMapByDirectionDialogQT.h
  gradientWidgetQT.h
  ShapePopulationMainWindowQT.h
  ShapePopulationQT.h
  )

set(QT_QRC_FILES
  ShapePopulationViewer.qrc
  )

if(ShapePopulationViewer_QT_VERSION VERSION_EQUAL "4")
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  QT4_ADD_RESOURCES(QRCrcs ${QT_QRC_FILES})
else()
  QT5_WRAP_UI(UISrcs ${UI_FILES})
  QT5_WRAP_CPP(MOCSrcs ${QT_WRAP})
  QT5_ADD_RESOURCES(QRCrcs ${QT_QRC_FILES})
endif()

# --- LIBRARIES ---------------------------------------------------------------------------
if(VTK_LIBRARIES)
  set(ShapePopulationLIBS ${VTK_LIBRARIES})
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
if( BUILD_TESTING )
  ADD_SUBDIRECTORY(Testing)
endif()

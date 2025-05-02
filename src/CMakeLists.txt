
# --- INCLUDES ----------------------------------------------------------------------------
set(_msg "Checking for export support")
message(STATUS "${_msg}")
string(FIND "${VTK_MODULES_ENABLED}" "GL2PS" index)
if( ${index} EQUAL -1)
  set(ShapePopulationViewer_HAS_EXPORT_SUPPORT 0)
  message(STATUS "${_msg} - no (GL2PS VTK module not enabled)")
else()
  set(ShapePopulationViewer_HAS_EXPORT_SUPPORT 1)
  message(STATUS "${_msg} - yes")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ShapePopulationViewerConfig.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/ShapePopulationViewerConfig.h
  )

# --- SRCS --------------------------------------------------------------------------------
set(CXX_FILES
  backgroundDialogQT.cxx
  cameraDialogQT.cxx
  CSVloaderQT.cxx
  timeSeriesLoaderQT.cxx
  customizeColorMapByDirectionDialogQT.cxx
  gradientArrow.cxx
  gradientWidgetQT.cxx
  ShapePopulationBase.cxx
  ShapePopulationColorMapIO.cxx
  ShapePopulationData.cxx
  ShapePopulationGradientWidgetQT.cxx
  ShapePopulationMainWindowQT.cxx
  ShapePopulationQT.cxx
  vtkPVPostFilter.cxx
  vtkPVPostFilterExecutive.cxx
  )

set(UI_FILES
  backgroundDialogQT.ui
  cameraDialogQT.ui
  CSVloaderQT.ui
  timeSeriesLoaderQT.ui
  customizeColorMapByDirectionDialogQT.ui
  ShapePopulationMainWindowQT.ui
  ShapePopulationQT.ui
  )

set(QT_WRAP
  backgroundDialogQT.h
  cameraDialogQT.h
  CSVloaderQT.h
  timeSeriesLoaderQT.h
  customizeColorMapByDirectionDialogQT.h
  gradientWidgetQT.h
  ShapePopulationGradientWidgetQT.h
  ShapePopulationMainWindowQT.h
  ShapePopulationQT.h
  )

set(QT_QRC_FILES
  ShapePopulationViewer.qrc
  )

# --- LIBRARIES ---------------------------------------------------------------------------
set(_moc_options)
if(ShapePopulationViewer_BUILD_SLICER_EXTENSION)
  set(_moc_options OPTIONS -DShapePopulationViewer_BUILD_SLICER_EXTENSION)
endif()

if(ShapePopulationViewer_QT_VERSION VERSION_EQUAL "5")
  QT5_WRAP_UI(UISrcs ${UI_FILES})
  QT5_WRAP_CPP(MOCSrcs ${QT_WRAP} ${_moc_options})
  QT5_ADD_RESOURCES(QRCrcs ${QT_QRC_FILES})
endif()
add_library(${LOCAL_PROJECT_NAME}Widget STATIC
  ${CXX_FILES}
  ${UISrcs}
  ${MOCSrcs}
  ${QRCrcs}
  )
target_link_libraries(${LOCAL_PROJECT_NAME}Widget PUBLIC
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES}
  $<$<BOOL:${ShapePopulationViewer_BUILD_SLICER_EXTENSION}>:CTKVisualizationVTKWidgets>
  )

if(ShapePopulationViewer_BUILD_SLICER_EXTENSION)
  set(MODULE_NAME ShapePopulationViewer)
  set(MODULE_TITLE ${MODULE_NAME})
  string(TOUPPER ${MODULE_NAME} MODULE_NAME_UPPER)

  set(MODULE_EXPORT_DIRECTIVE "Q_SLICER_QTMODULES_${MODULE_NAME_UPPER}_EXPORT")

  # Current_{source,binary} and Slicer_{Libs,Base} already included
  set(MODULE_INCLUDE_DIRECTORIES
    ${vtkSlicerSRepModuleMRML_INCLUDE_DIRS}
    ${vtkSlicerSRepModuleLogic_INCLUDE_DIRS}
    )
  set(MODULE_SRCS
    qSlicer${MODULE_NAME}LayoutViewFactory.cxx
    qSlicer${MODULE_NAME}LayoutViewFactory.h
    qSlicer${MODULE_NAME}Module.cxx
    qSlicer${MODULE_NAME}Module.h
    qSlicer${MODULE_NAME}ModuleWidget.cxx
    qSlicer${MODULE_NAME}ModuleWidget.h
    )
  set(MODULE_MOC_SRCS
    qSlicer${MODULE_NAME}LayoutViewFactory.h
    qSlicer${MODULE_NAME}Module.h
    qSlicer${MODULE_NAME}ModuleWidget.h
    )
  set(MODULE_UI_SRCS
    qSlicer${MODULE_NAME}ModuleWidget.ui
    )
  set(MODULE_TARGET_LIBRARIES
    ${LOCAL_PROJECT_NAME}Widget
    vtkSlicerSRepModuleMRML
    vtkSlicerSRepModuleLogic
    )
  set(MODULE_RESOURCES
    )

  slicerMacroBuildLoadableModule(
    NAME ${MODULE_NAME}
    TITLE ${MODULE_TITLE}
    EXPORT_DIRECTIVE ${MODULE_EXPORT_DIRECTIVE}
    INCLUDE_DIRECTORIES ${MODULE_INCLUDE_DIRECTORIES}
    SRCS ${MODULE_SRCS}
    MOC_SRCS ${MODULE_MOC_SRCS}
    UI_SRCS ${MODULE_UI_SRCS}
    TARGET_LIBRARIES ${MODULE_TARGET_LIBRARIES}
    RESOURCES ${MODULE_RESOURCES}
    #WITH_GENERIC_TESTS
    )
else()

# --- SEMMacro ----------------------------------------------------------------------------
SEMMacroBuildCLI(
  NAME ${LOCAL_PROJECT_NAME}
  EXECUTABLE_ONLY
  ADDITIONAL_SRCS
    ShapePopulationViewer.cxx
  TARGET_LIBRARIES
    ${LOCAL_PROJECT_NAME}Widget
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )

endif()

# --- CTest -------------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_TESTING )
  ADD_SUBDIRECTORY(Testing)
endif()

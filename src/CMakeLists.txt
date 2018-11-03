# --- cmake -------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

# --- INCLUDES ----------------------------------------------------------------------------
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

find_package(VTK REQUIRED )
include(${VTK_USE_FILE})
string(FIND "${VTK_MODULES_ENABLED}" "GL2PS" index)

if( ${index} EQUAL -1)
  add_definitions("-DSPV_EXTENSION=1")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ShapePopulationBase.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/ShapePopulationBase.h
  )

# --- SRCS --------------------------------------------------------------------------------
file(GLOB CXX_FILES *.cxx)
file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *QT.h)
file(GLOB QT_QRC_FILES *.qrc)

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
#set(BUILD_TESTING ON CACHE BOOL "Build, configure and copy testing files")

if( BUILD_TESTING )
  ADD_SUBDIRECTORY(Testing)
endif()



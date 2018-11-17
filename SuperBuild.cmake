

set(EXTENSION_BUILD_SUBDIRECTORY ${LOCAL_PROJECT_NAME}-build)

#-----------------------------------------------------------------------------
find_package(Git REQUIRED)

#-----------------------------------------------------------------------------
# Superbuild option(s)
#-----------------------------------------------------------------------------
option(USE_SYSTEM_ITK "Build using an externally defined version of ITK" OFF)
set(ShapePopulationViewer_USE_SYSTEM_ITK ${USE_SYSTEM_ITK})

option(USE_SYSTEM_SlicerExecutionModel "Build using an externally defined version of SlicerExecutionModel"  OFF)
set(ShapePopulationViewer_USE_SYSTEM_SlicerExecutionModel ${USE_SYSTEM_SlicerExecutionModel})

option(USE_SYSTEM_VTK "Build using an externally defined version of VTK" OFF)
set(ShapePopulationViewer_USE_SYSTEM_VTK ${USE_SYSTEM_VTK})

#-----------------------------------------------------------------------------
# Top-level "external" project
#-----------------------------------------------------------------------------

# Project dependencies
set(${LOCAL_PROJECT_NAME}_DEPENDS
  )
if(NOT ShapePopulationViewer_BUILD_SLICER_EXTENSION)
  list(APPEND ${LOCAL_PROJECT_NAME}_DEPENDS
    ITK
    SlicerExecutionModel
    VTK
    )
endif()

set(proj ${SUPERBUILD_TOPLEVEL_PROJECT})

ExternalProject_Include_Dependencies(${proj}
  PROJECT_VAR proj
  SUPERBUILD_VAR ${LOCAL_PROJECT_NAME}_SUPERBUILD
  )

ExternalProject_Add(${proj}
  ${${proj}_EP_ARGS}
  DOWNLOAD_COMMAND ""
  INSTALL_COMMAND ""
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
  BINARY_DIR ${EXTENSION_BUILD_SUBDIRECTORY}
  CMAKE_CACHE_ARGS
    # Compiler settings
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
    -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}
    -DCMAKE_CXX_EXTENSIONS:BOOL=${CMAKE_CXX_EXTENSIONS}
    # Superbuild
    -DEXTENSION_SUPERBUILD_BINARY_DIR:PATH=${${EXTENSION_NAME}_BINARY_DIR}
    -D${LOCAL_PROJECT_NAME}_SUPERBUILD:BOOL=OFF
  INSTALL_COMMAND ""
  DEPENDS
    ${${LOCAL_PROJECT_NAME}_DEPENDS}
  )

ExternalProject_AlwaysConfigure(${proj})

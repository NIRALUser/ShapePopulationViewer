cmake_minimum_required(VERSION 2.8.3)
cmake_policy(VERSION 2.8.3)

project(ShapePopulationViewer)

set(LOCAL_PROJECT_NAME ShapePopulationViewer)

#-----------------------------------------------------------------------------
# Update CMake module path
# We need to update the CMake Module path in this main CMakeLists.txt file
# so that we can include SlicerExtensionsConfigureMacros which is in the current
# ${Project}/CMake folder
#------------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Update CMake module path
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/SuperBuild
  ${CMAKE_MODULE_PATH}
  )

include(${CMAKE_CURRENT_SOURCE_DIR}/Common.cmake)

## NOTE THERE SHOULD BE NO PROJECT STATEMENT HERE!
## This file acts as a simple switch to initiate
## two completely independant CMake build environments.

#-----------------------------------------------------------------------------
# Standalone vs Slicer extension option
#

# This option should be named after the project name, it corresponds to the
# option set to ON when the project is build by the Slicer Extension build
# system.

set(_default OFF)
set(_reason "${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION is ON")
if(NOT DEFINED ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION AND DEFINED Slicer_DIR)
  set(_default ON)
  set(_reason "Slicer_DIR is SET")
endif()

option(${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION "Build as a Slicer Extension" ${_default})

set(_msg "Checking if building as a Slicer extension")
message(STATUS ${_msg})
if(${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
  message(STATUS "${_msg} - yes (${_reason})")
else()
  message(STATUS "${_msg} - no (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION is OFF)")
endif()

#-----------------------------------------------------------------------------
# Superbuild Option - Enabled by default
#                   Phase I:  ${LOCAL_PROJECT_NAME}_SUPERBUILD is set to ON, and the
#                             supporting packages defined in "SuperBuild.cmake"
#                             are built.  The last package in "SuperBuild.cmake"
#                             to be built is a recursive call to this
#                             file with ${LOCAL_PROJECT_NAME}_SUPERBUILD explicitly
#                             set to "OFF" to initiate Phase II
#
#                   Phase II: Build the ${LOCAL_PROJECT_NAME}, referencing the support
#                             packages built in Phase I.
#-----------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_SLICER_EXTENSION )
  set(${LOCAL_PROJECT_NAME}_SUPERBUILD OFF CACHE STRING "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake.")
endif()
option(${LOCAL_PROJECT_NAME}_SUPERBUILD "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake." ON)

#-----------------------------------------------------------------------------
# Superbuild script
#-----------------------------------------------------------------------------
if(${LOCAL_PROJECT_NAME}_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
else()
  include("${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_PROJECT_NAME}.cmake")
  return()
endif()

message(FATAL_ERROR "You should never reach this point !")

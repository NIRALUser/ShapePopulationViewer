cmake_minimum_required(VERSION 3.5)

# C++ standard
set(_msg "Setting C++ standard")
message(STATUS "${_msg}")
if(NOT DEFINED CMAKE_CXX_STANDARD)
  if("${ShapePopulationViewer_VTK_VERSION_MAJOR}" STREQUAL "8" OR DEFINED Qt5_DIR OR DEFINED Slicer_DIR)
    set(CMAKE_CXX_STANDARD 11)
  endif()
  if(DEFINED Slicer_DIR)
    set(CMAKE_CXX_STANDARD 17)
  endif()
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "${_msg} - C++${CMAKE_CXX_STANDARD}")

# Update CMake module path
set(ShapePopulationViewer_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
list(INSERT CMAKE_MODULE_PATH 0 ${ShapePopulationViewer_CMAKE_DIR})

# Superbuild
set(SUPERBUILD_TOPLEVEL_PROJECT ShapePopulationViewer)
set(EXTERNAL_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild)
set(EXTERNAL_PROJECT_FILE_PREFIX "External_")
include(ExternalProject)
include(ExternalProjectDependency)

project(ShapePopulationViewer)
set(LOCAL_PROJECT_NAME ShapePopulationViewer)
set(EXTENSION_NAME "${LOCAL_PROJECT_NAME}")

## This file acts as a simple switch to initiate
## two completely independant CMake build environments.

#-----------------------------------------------------------------------------
# Standalone vs Slicer extension option
#-----------------------------------------------------------------------------

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
mark_as_superbuild(${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION:BOOL)

#-----------------------------------------------------------------------------
# Slicer
#-----------------------------------------------------------------------------
if(${EXTENSION_NAME}_BUILD_SLICER_EXTENSION)
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
endif()

#-----------------------------------------------------------------------------
# Qt
#-----------------------------------------------------------------------------

set(_qt_version "5")
set(ShapePopulationViewer_QT_VERSION ${_qt_version} CACHE STRING "Version of Qt to use")
set_property(CACHE ShapePopulationViewer_QT_VERSION PROPERTY STRINGS 5)

# Select VTK major version based on Qt version
if(ShapePopulationViewer_QT_VERSION STREQUAL "5")
  set(ShapePopulationViewer_VTK_VERSION_MAJOR "8")
else()
  message(FATAL_ERROR "error: Unsupported ShapePopulationViewer_QT_VERSION: ${ShapePopulationViewer_QT_VERSION}")
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

include(${CMAKE_CURRENT_SOURCE_DIR}/Common.cmake) #<-- All feature options for top superbuild and inner product build

set(_default ON)
if(${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
  set(_default OFF)
endif()

option(${LOCAL_PROJECT_NAME}_SUPERBUILD "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake." ${_default})

set(_msg "Checking if enabling Superbuild")
message(STATUS ${_msg})
if(${LOCAL_PROJECT_NAME}_SUPERBUILD)
  message(STATUS "${_msg} - yes")
else()
  message(STATUS "${_msg} - no")
endif()

if(${LOCAL_PROJECT_NAME}_SUPERBUILD)
  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake") #<-- Harness for managing top superbuild, and finally delegate inner product build
  return()
else()
  include("${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_PROJECT_NAME}.cmake") #<-- inner product build (a separate build environment from top level build)
  return()
endif()

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
set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/SuperBuild
  ${CMAKE_MODULE_PATH}
  )
#-----------------------------------------------------------------------------
# SLICER EXTENSION
#-----------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_SLICER_EXTENSION )
    include(SlicerExtensionsConfigureMacros)
    include(CMakeParseArguments)
    ADD_DEFINITIONS(-DSPV_EXTENSION=1)
    #VTK_VERSION_MAJOR is define but not a CACHE variable
    set( VTK_VERSION_MAJOR ${VTK_VERSION_MAJOR} CACHE STRING "Choose the expected VTK major version to build Slicer (5 or 6).")
    set(EXTENSION_NAME ${LOCAL_PROJECT_NAME} )
    set(MODULE_NAME ${LOCAL_PROJECT_NAME} )
    set(EXTENSION_HOMEPAGE "https://www.nitrc.org/projects/shapepopviewer/")
    set(EXTENSION_CATEGORY "Shape Analysis")
    set(EXTENSION_CONTRIBUTORS "Alexis Girault (NIRAL, UNC)")
    set(EXTENSION_DESCRIPTION "Visualize and interact with multiple surfaces at the same time to easily compare them")
    set(EXTENSION_ICONURL "https://www.nitrc.org/project/screenshot.php?group_id=759&screenshot_id=704")
    set(EXTENSION_SCREENSHOTURLS "http://wiki.na-mic.org/Wiki/images/8/81/Screenshot1.png http://wiki.na-mic.org/Wiki/images/1/13/SPVscreenshot2.png")
    set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
    set(EXTENSION_BUILD_SUBDIRECTORY DTIAtlasBuilder-build)
    set(${LOCAL_PROJECT_NAME}_SUPERBUILD OFF CACHE BOOL "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake.")
    set(EXTENSION_STATUS Release)
endif()

## NOTE THERE SHOULD BE NO PROJECT STATEMENT HERE!
## This file acts as a simple switch to initiate
## two completely independant CMake build environments.

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
option(${LOCAL_PROJECT_NAME}_SUPERBUILD "Build ${LOCAL_PROJECT_NAME} and the projects it depends on via SuperBuild.cmake." ON)

#-----------------------------------------------------------------------------
# Superbuild script
#-----------------------------------------------------------------------------
if(${LOCAL_PROJECT_NAME}_SUPERBUILD)

  include("${CMAKE_CURRENT_SOURCE_DIR}/SuperBuild.cmake")
  return()
else()

  if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
    unsetForSlicer(NAMES CMAKE_MODULE_PATH CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS )
    find_package(Slicer REQUIRED)
    include(${Slicer_USE_FILE})
    #VTK_VERSION_MAJOR is define but not a CACHE variable
    set( VTK_VERSION_MAJOR ${VTK_VERSION_MAJOR} CACHE STRING "Choose the expected VTK major version to build Slicer (5 or 6).")
  endif()

  include("${CMAKE_CURRENT_SOURCE_DIR}/${LOCAL_PROJECT_NAME}.cmake")

  if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
    include(${Slicer_EXTENSION_CPACK})
  endif()

  return()
endif()

message(FATAL_ERROR "You should never reach this point !")

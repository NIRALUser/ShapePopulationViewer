#-----------------------------------------------------------------------------
# SLICER EXTENSION
#-----------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_SLICER_EXTENSION )
  ADD_DEFINITIONS(-DSPV_EXTENSION=1)
  #VTK_VERSION_MAJOR is define but not a CACHE variable
  set( VTK_VERSION_MAJOR ${VTK_VERSION_MAJOR} CACHE STRING "Choose the expected VTK major version to build Slicer (5 or 6).")
  set(EXTENSION_NAME ${LOCAL_PROJECT_NAME} )
  set(MODULE_NAME ${LOCAL_PROJECT_NAME} )
  set(EXTENSION_HOMEPAGE "https://www.nitrc.org/projects/shapepopviewer/")
  set(EXTENSION_CATEGORY "Shape Analysis")
  set(EXTENSION_CONTRIBUTORS "Alexis Girault (NIRAL, UNC), Francois Budin (NIRAL, UNC), Laura Pascal (DCBIA-OrthoLab, UofM)")
  set(EXTENSION_DESCRIPTION "Visualize and interact with multiple surfaces at the same time to easily compare them")
  set(EXTENSION_ICONURL "https://www.nitrc.org/project/screenshot.php?group_id=759&screenshot_id=704")
  set(EXTENSION_SCREENSHOTURLS "http://wiki.na-mic.org/Wiki/images/8/81/Screenshot1.png http://wiki.na-mic.org/Wiki/images/1/13/SPVscreenshot2.png")
  set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
  set(EXTENSION_BUILD_SUBDIRECTORY .)
  unsetForSlicer( NAMES QT_QMAKE_EXECUTABLE SlicerExecutionModel_DIR ITK_DIR VTK_DIR CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS ITK_LIBRARIES )
  find_package(Slicer REQUIRED)
  include(${Slicer_USE_FILE})
#  resetForSlicer( NAMES VTK_DIR CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS VTK_LIBRARIES VTK_VERSION_MAJOR  )
  resetForSlicer( NAMES CMAKE_C_COMPILER CMAKE_CXX_COMPILER CMAKE_CXX_FLAGS CMAKE_C_FLAGS )
endif()

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
if( BUILD_TESTING )
  include(CTest)
  include(ExternalData)
endif()


add_subdirectory(src)

if( ShapePopulationViewer_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
endif()

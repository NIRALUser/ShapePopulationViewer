#-----------------------------------------------------------------------------
# SLICER EXTENSION
#-----------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_SLICER_EXTENSION )
  set(EXTENSION_NAME "ShapePopulationViewer")
  set(EXTENSION_HOMEPAGE "https://www.nitrc.org/projects/shapepopviewer/")
  set(EXTENSION_CATEGORY "Shape Visualization")
  set(EXTENSION_CONTRIBUTORS "Alexis Girault (NIRAL, UNC), Francois Budin (NIRAL, UNC), Laura Pascal (DCBIA-OrthoLab, UofM)")
  set(EXTENSION_DESCRIPTION "Visualize and interact with multiple surfaces at the same time to easily compare them")
  set(EXTENSION_ICONURL "https://www.nitrc.org/project/screenshot.php?group_id=759&screenshot_id=704")
  set(EXTENSION_SCREENSHOTURLS "https://www.na-mic.org/w/img_auth.php/8/81/Screenshot1.png https://www.na-mic.org/w/img_auth.php/1/13/SPVscreenshot2.png")
  set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any
  set(EXTENSION_BUILD_SUBDIRECTORY .)
endif()

#-----------------------------------------------------------------------------
# Prequisites
#-----------------------------------------------------------------------------
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

if(ShapePopulationViewer_QT_VERSION VERSION_EQUAL "5")
  set(qt_components Core Widgets)
  if(UNIX AND NOT APPLE)
    list(APPEND qt_components X11Extras)
  endif()
  find_package(Qt5 COMPONENTS ${qt_components} REQUIRED)
  set(QT_LIBRARIES)
  foreach(lib IN LISTS qt_components)
    list(APPEND QT_LIBRARIES Qt5::${lib})
  endforeach()
endif()

#-----------------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_TESTING )
  include(CTest)
  include(ExternalData)
endif()

#-----------------------------------------------------------------------------
add_subdirectory(src)

#-----------------------------------------------------------------------------
# Packaging
#-----------------------------------------------------------------------------
if( ShapePopulationViewer_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_GENERATE_CONFIG})
  include(${Slicer_EXTENSION_CPACK})
endif()

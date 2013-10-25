

# --- cmake -------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.3)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# --- INCLUDES ----------------------------------------------------------------------------
find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
#find_package(Qt4 REQUIRED)
#include(${QT_USE_FILE})        #-- In Common.cmake

# --- SRCS --------------------------------------------------------------------------------
file(GLOB CXX_FILES *.cxx)
file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *QT.h)
QT4_WRAP_UI(UISrcs ${UI_FILES})
QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})

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
    ADDITIONAL_SRCS ${CXX_FILES} ${UISrcs} ${MOCSrcs}
    TARGET_LIBRARIES ${ShapePopulationLIBS}
    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    INSTALL_RUNTIME_DESTINATION bin
    INSTALL_LIBRARY_DESTINATION lib
    INSTALL_ARCHIVE_DESTINATION lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)


# --- CTest -------------------------------------------------------------------------------
#set(BUILD_TESTING ON CACHE BOOL "Build, configure and copy testing files")

#if( BUILD_TESTING )
#    enable_testing()
#    add_subdirectory( Testing )
#endif()



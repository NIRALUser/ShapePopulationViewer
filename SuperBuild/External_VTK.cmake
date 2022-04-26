
set(proj VTK)

# Set dependency list
set(${proj}_DEPENDENCIES
  Qt${ShapePopulationViewer_QT_VERSION}
  )

# Include dependent projects if any
ExternalProject_Include_Dependencies(${proj} PROJECT_VAR proj DEPENDS_VAR ${proj}_DEPENDENCIES)

if(${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})
  unset(VTK_DIR CACHE)
  unset(VTK_SOURCE_DIR CACHE)
  find_package(VTK REQUIRED NO_MODULE)
endif()

# Sanity checks
if(DEFINED VTK_DIR AND NOT EXISTS ${VTK_DIR})
  message(FATAL_ERROR "VTK_DIR variable is defined but corresponds to nonexistent directory")
endif()

if(DEFINED VTK_SOURCE_DIR AND NOT EXISTS ${VTK_SOURCE_DIR})
  message(FATAL_ERROR "VTK_SOURCE_DIR variable is defined but corresponds to nonexistent directory")
endif()

if((NOT DEFINED VTK_DIR OR NOT DEFINED VTK_SOURCE_DIR) AND NOT ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})

  set(EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS)

  list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
    -DVTK_USE_GUISUPPORT:BOOL=ON
    -DVTK_USE_QVTK_QTOPENGL:BOOL=ON
    -DModule_vtkTestingRendering:BOOL=ON
    )
  if(ShapePopulationViewer_QT_VERSION VERSION_EQUAL "5")
    set(ShapePopulationViewer_VTK_RENDERING_BACKEND "OpenGL2")
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
      -DVTK_QT_VERSION:STRING=5
      -DVTK_Group_Qt:BOOL=ON
      -DQt5_DIR:FILEPATH=${Qt5_DIR}
      )
  endif()
  if("${ShapePopulationViewer_VTK_RENDERING_BACKEND}" STREQUAL "OpenGL2")
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
      -DModule_vtkGUISupportQtOpenGL:BOOL=ON
    )
  endif()
  if(APPLE)
    list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
      -DVTK_USE_CARBON:BOOL=OFF
      -DVTK_USE_COCOA:BOOL=ON # Default to Cocoa, VTK/CMakeLists.txt will enable Carbon and disable cocoa if needed
      -DVTK_USE_X:BOOL=OFF
      -DVTK_REQUIRED_OBJCXX_FLAGS:STRING=
      #-DVTK_USE_RPATH:BOOL=ON # Unused
      )
  endif()
  if(UNIX AND NOT APPLE)
    find_package(FontConfig QUIET)
    if(FONTCONFIG_FOUND)
      list(APPEND EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS
        -DModule_vtkRenderingFreeTypeFontConfig:BOOL=ON
        )
    endif()
  endif()

  ExternalProject_SetIfNotDefined(
    ${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_REPOSITORY
    "${EP_GIT_PROTOCOL}://github.com/slicer/VTK.git"
    QUIET
    )

set(_git_tag)
if("${ShapePopulationViewer_VTK_VERSION_MAJOR}" STREQUAL "8")
  set(_git_tag "75414fe171e5ae2ed6e4df608deb1d578d9ec7c3")
else()
  message(FATAL_ERROR "error: Unsupported ShapePopulationViewer_VTK_VERSION_MAJOR: ${ShapePopulationViewer_VTK_VERSION_MAJOR}")
endif()
  ExternalProject_SetIfNotDefined(
    ${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_TAG
    ${_git_tag}
    QUIET
    )

  set(EP_SOURCE_DIR ${CMAKE_BINARY_DIR}/${proj})
  set(EP_BINARY_DIR ${CMAKE_BINARY_DIR}/${proj}-build)

  ExternalProject_Add(${proj}
    ${${proj}_EP_ARGS}
    GIT_REPOSITORY "${${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_REPOSITORY}"
    GIT_TAG "${${SUPERBUILD_TOPLEVEL_PROJECT}_${proj}_GIT_TAG}"
    SOURCE_DIR ${EP_SOURCE_DIR}
    BINARY_DIR ${EP_BINARY_DIR}
    CMAKE_CACHE_ARGS
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
      -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}
      -DCMAKE_CXX_EXTENSIONS:BOOL=${CMAKE_CXX_EXTENSIONS}
      -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
      -DBUILD_TESTING:BOOL=OFF
      -DBUILD_EXAMPLES:BOOL=OFF
      -DBUILD_SHARED_LIBS:BOOL=OFF
      -DVTK_USE_PARALLEL:BOOL=ON
      -DVTK_DEBUG_LEAKS:BOOL=${VTK_DEBUG_LEAKS}
      -DVTK_LEGACY_REMOVE:BOOL=ON
      -DVTK_WRAP_TCL:BOOL=OFF
      #-DVTK_USE_RPATH:BOOL=ON # Unused
      -DVTK_WRAP_PYTHON:BOOL=OFF
      -DVTK_Group_Qt:BOOL=ON
      -DVTK_USE_SYSTEM_ZLIB:BOOL=OFF
      -DVTK_ENABLE_KITS:BOOL=ON
      -DVTK_RENDERING_BACKEND:STRING=${ShapePopulationViewer_VTK_RENDERING_BACKEND}
      -DVTK_SMP_IMPLEMENTATION_TYPE:STRING=Sequential
      ${EXTERNAL_PROJECT_OPTIONAL_CMAKE_CACHE_ARGS}
    INSTALL_COMMAND ""
    DEPENDS
      ${${proj}_DEPENDENCIES}
    )

  set(VTK_DIR ${EP_BINARY_DIR})
  set(VTK_SOURCE_DIR ${EP_SOURCE_DIR})

else()
  ExternalProject_Add_Empty(${proj} DEPENDS ${${proj}_DEPENDENCIES})
endif()

mark_as_superbuild(VTK_SOURCE_DIR:PATH)

mark_as_superbuild(
  VARS VTK_DIR:PATH
  )

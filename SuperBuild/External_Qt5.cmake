
set(proj Qt5)

# Set dependency list
set(${proj}_DEPENDENCIES "")

# Include dependent projects if any
ExternalProject_Include_Dependencies(${proj} PROJECT_VAR proj DEPENDS_VAR ${proj}_DEPENDENCIES)

# By default, lookup Qt on the system
if(NOT DEFINED Qt5_DIR)
  set(${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj} ON)
endif()

if(${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})
  unset(Qt5_DIR CACHE)
  find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
endif()

# Sanity checks
if(DEFINED Qt5_DIR AND NOT EXISTS ${Qt5_DIR})
  message(FATAL_ERROR "Qt5_DIR variable is defined but corresponds to nonexistent directory")
endif()

if(NOT DEFINED Qt5_DIR AND NOT ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})
  message(FATAL_ERROR "Build ${proj} from source is not supported. Set either Qt5_DIR or ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj}")
else()
  ExternalProject_Add_Empty(${proj} DEPENDS ${${proj}_DEPENDENCIES})
endif()

mark_as_superbuild(
  VARS Qt5_DIR:PATH
  )

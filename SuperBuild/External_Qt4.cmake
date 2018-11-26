
set(proj Qt4)

# Set dependency list
set(${proj}_DEPENDENCIES "")

# Include dependent projects if any
ExternalProject_Include_Dependencies(${proj} PROJECT_VAR proj DEPENDS_VAR ${proj}_DEPENDENCIES)

# By default, lookup Qt on the system
if(NOT DEFINED QT_QMAKE_EXECUTABLE)
  set(${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj} ON)
endif()

if(${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})
  unset(QT_QMAKE_EXECUTABLE CACHE)
  find_package(Qt4 REQUIRED)
endif()

# Sanity checks
if(DEFINED QT_QMAKE_EXECUTABLE AND NOT EXISTS ${QT_QMAKE_EXECUTABLE})
  message(FATAL_ERROR "QT_QMAKE_EXECUTABLE variable is defined but corresponds to nonexistent file")
endif()

if(NOT DEFINED QT_QMAKE_EXECUTABLE AND NOT ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj})
  message(FATAL_ERROR "Build ${proj} from source is not supported. Set either QT_QMAKE_EXECUTABLE or ${SUPERBUILD_TOPLEVEL_PROJECT}_USE_SYSTEM_${proj}")
else()
  ExternalProject_Add_Empty(${proj} DEPENDS ${${proj}_DEPENDENCIES})
endif()

mark_as_superbuild(
  VARS QT_QMAKE_EXECUTABLE:PATH
  )


# Add external data from midas
list(APPEND ExternalData_URL_TEMPLATES
"https://github.com/Slicer/SlicerTestingData/releases/download/%(algo)/%(hash)"
)

# Set the data
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)
set(sphere DATA{${SOURCE_DIR}/sphereExpectedResult.vtk})
set(rightCondyle DATA{${SOURCE_DIR}/rightCondyle.vtk})
set(rightCondyleExpectedResult DATA{${SOURCE_DIR}/rightCondyleExpectedResult.vtk})
set(rightCondyleExpectedResult2 DATA{${SOURCE_DIR}/rightCondyleExpectedResult2.vtk})
set(rightCondyle2 DATA{${SOURCE_DIR}/rightCondyle_signedclosestpoint.vtk})

# Test launcher
set(ShapePopulationViewer_LAUNCH_COMMAND )
if(DEFINED Slicer_LAUNCH_COMMAND)
  set(ShapePopulationViewer_LAUNCH_COMMAND ${Slicer_LAUNCH_COMMAND})
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Test 1 of creationSphereActor in the class ShapePopulationBase
add_executable(TestCreationSphereActor mainTestCreationSphere.cxx testCreationSphere.cxx)
target_link_libraries(TestCreationSphereActor ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_CreationSphereActor
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestCreationSphereActor> ${sphere}
)

# Test 2 of ChangeView in the class ShapePopulationBase
add_executable(TestChangeView mainTestChangeView.cxx testChangeView.cxx)
target_link_libraries(TestChangeView ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_ChangeView
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestChangeView> ${rightCondyle}
)

# Test 3 of setMeshOpacity in the class ShapePopulationBase
add_executable(TestMeshOpacity mainTestMeshOpacity.cxx testMeshOpacity.cxx)
target_link_libraries(TestMeshOpacity ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setMeshOpacity
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestMeshOpacity> ${rightCondyle}
)

# Test 4 of setVectorScale in the class ShapePopulationBase
add_executable(TestVectorScale mainTestVectorScale.cxx testVectorScale.cxx)
target_link_libraries(TestVectorScale ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setVectorScale
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestVectorScale> ${rightCondyle}
)

# Test 5 of computeCommonRange in the class ShapePopulationBase
add_executable(TestComputeCommonRange mainTestComputeCommonRange.cxx testComputeCommonRange.cxx)
target_link_libraries(TestComputeCommonRange ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_computeCommonRange
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestComputeCommonRange> ${rightCondyle}
)

# Test 6 of UpdateColorMapByDirection in the class ShapePopulationBase
add_executable(TestUpdateColorMapByDirection mainTestUpdateColorMapByDirection.cxx testUpdateColorMapByDirection.cxx)
target_link_libraries(TestUpdateColorMapByDirection ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_UpdateColorMapByDirection
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestUpdateColorMapByDirection> ${rightCondyle} ${rightCondyleExpectedResult}
)

# Test 7 of setBackgroundSelectedColor in the class ShapePopulationBase
add_executable(TestBackgroundSelectedColor mainTestBackgroundSelectedColor.cxx testBackgroundSelectedColor.cxx)
target_link_libraries(TestBackgroundSelectedColor ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setBackgroundSelectedColor
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestBackgroundSelectedColor> ${rightCondyle}
)

# Test 8 of setLabelColor in the class ShapePopulationBase
add_executable(TestLabelColor mainTestLabelColor.cxx testLabelColor.cxx)
target_link_libraries(TestLabelColor ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setLabelColor
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestLabelColor> ${rightCondyle}
)

# Test 9 of setBackgroundUnselectedColor in the class ShapePopulationBase
add_executable(TestBackgroundUnselectedColor mainTestBackgroundUnselectedColor.cxx testBackgroundUnselectedColor.cxx)
target_link_libraries(TestBackgroundUnselectedColor ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setBackgroundUnselectedColor
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestBackgroundUnselectedColor> ${rightCondyle}
)

# Test 10 of creationSphereWidget in the class ShapePopulationBase
add_executable(TestCreationSphereWidget mainTestCreationSphereWidget.cxx testCreationSphereWidget.cxx)
target_link_libraries(TestCreationSphereWidget ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_creationSphereWidget
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestCreationSphereWidget> ${sphere}
)

# Test 11 of displayColorbar in the class ShapePopulationBase
add_executable(TestDisplayColorbar mainTestDisplayColorbar.cxx testDisplayColorbar.cxx)
target_link_libraries(TestDisplayColorbar ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayColorbar
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestDisplayColorbar> ${rightCondyle}
)

# Test 12 of displayAttribute in the class ShapePopulationBase
add_executable(TestDisplayAttribute mainTestDisplayAttribute.cxx testDisplayAttribute.cxx)
target_link_libraries(TestDisplayAttribute ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayAttribute
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestDisplayAttribute> ${rightCondyle}
)

# Test 13 of displayMeshName in the class ShapePopulationBase
add_executable(TestDisplayMeshName mainTestDisplayMeshName.cxx testDisplayMeshName.cxx)
target_link_libraries(TestDisplayMeshName ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayMeshName
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestDisplayMeshName> ${rightCondyle}
)

# Test 14 of displayVectors in the class ShapePopulationBase
add_executable(TestDisplayVectors mainTestDisplayVectors.cxx testDisplayVectors.cxx)
target_link_libraries(TestDisplayVectors ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayVectors
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestDisplayVectors> ${rightCondyle}
)

# Test 15 of computeCommonAttributes in the class ShapePopulationBase
add_executable(TestComputeCommonAttributes mainTestComputeCommonAttributes.cxx testComputeCommonAttributes.cxx)
target_link_libraries(TestComputeCommonAttributes ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_computeCommonAttributes
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestComputeCommonAttributes> ${rightCondyle} ${rightCondyle2}
)

# Test 16 of displayColorMapByMagnitude and displayColorMapByDirection in the class ShapePopulationBase
add_executable(TestDisplayColorMap mainTestDisplayColorMap.cxx testDisplayColorMap.cxx)
target_link_libraries(TestDisplayColorMap ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayColorMap
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestDisplayColorMap> ${rightCondyle} ${rightCondyleExpectedResult2}
)

# Test 17 of UpdateAttribute in the class ShapePopulationBase
add_executable(TestUpdateAttribute mainTestUpdateAttribute.cxx testUpdateAttribute.cxx)
target_link_libraries(TestUpdateAttribute ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_UpdateAttribute
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestUpdateAttribute> ${rightCondyle} ${rightCondyleExpectedResult2}
)

# Test 18 of UpdateColorMapByMagnitude in the class ShapePopulationBase
add_executable(TestUpdateColorMapByMagnitude mainTestUpdateColorMapByMagnitude.cxx testUpdateColorMapByMagnitude.cxx)
target_link_libraries(TestUpdateColorMapByMagnitude ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_UpdateColorMapByMagnitude
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestUpdateColorMapByMagnitude> ${rightCondyle}
)

# Test 19 of the color mode of vectors in the class ShapePopulationBase
add_executable(TestVectorsColorMode mainTestVectorsColorMode.cxx testVectorsColorMode.cxx)
target_link_libraries(TestVectorsColorMode ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_VectorsColorMode
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestVectorsColorMode> ${rightCondyle}
)

# Test 20 of getSelectedIndex in the class ShapePopulationBase
add_executable(TestSelectedIndex mainTestSelectedIndex.cxx testSelectedIndex.cxx)
target_link_libraries(TestSelectedIndex ShapePopulationViewerWidget)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_getSelectedIndex
        COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:TestSelectedIndex> ${rightCondyle}
)

# Test for the command --help
if(TARGET ShapePopulationViewer)
  add_test(
          NAME PrintHelp
          COMMAND ${ShapePopulationViewer_LAUNCH_COMMAND} $<TARGET_FILE:ShapePopulationViewer> --help
  )
endif()

ExternalData_Add_Target(MY_DATA)

cmake_minimum_required(VERSION 2.8.3)

# Add external data from midas
list(APPEND ExternalData_URL_TEMPLATES
"http://slicer.kitware.com/midas3/api/rest?method=midas.bitstream.download&checksum=%(hash)"
)

# Set the data
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)
set(sphere DATA{${SOURCE_DIR}/sphereExpectedResult.vtk})
set(rightCondyle DATA{${SOURCE_DIR}/rightCondyle.vtk})
set(rightCondyleExpectedResult DATA{${SOURCE_DIR}/rightCondyleExpectedResult.vtk})
set(rightCondyleExpectedResult2 DATA{${SOURCE_DIR}/rightCondyleExpectedResult2.vtk})
set(rightCondyle2 DATA{${SOURCE_DIR}/rightCondyle_signedclosestpoint.vtk})

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Link directories
link_directories(${CMAKE_BINARY_DIR}/lib)

# Test 1 of creationSphereActor in the class ShapePopulationBase
add_executable(TestCreationSphereActor mainTestCreationSphere.cxx testCreationSphere.cxx)
target_link_libraries(TestCreationSphereActor ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_CreationSphereActor
        COMMAND $<TARGET_FILE:TestCreationSphereActor> ${sphere}
)

# Test 2 of ChangeView in the class ShapePopulationBase
add_executable(TestChangeView mainTestChangeView.cxx testChangeView.cxx)
target_link_libraries(TestChangeView ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_ChangeView
        COMMAND $<TARGET_FILE:TestChangeView> ${rightCondyle}
)

# Test 3 of setMeshOpacity in the class ShapePopulationBase
add_executable(TestMeshOpacity mainTestMeshOpacity.cxx testMeshOpacity.cxx)
target_link_libraries(TestMeshOpacity ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setMeshOpacity
        COMMAND $<TARGET_FILE:TestMeshOpacity> ${rightCondyle}
)

# Test 4 of setVectorScale in the class ShapePopulationBase
add_executable(TestVectorScale mainTestVectorScale.cxx testVectorScale.cxx)
target_link_libraries(TestVectorScale ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setVectorScale
        COMMAND $<TARGET_FILE:TestVectorScale> ${rightCondyle}
)

# Test 5 of computeCommonRange in the class ShapePopulationBase
add_executable(TestComputeCommonRange mainTestComputeCommonRange.cxx testComputeCommonRange.cxx)
target_link_libraries(TestComputeCommonRange ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_computeCommonRange
        COMMAND $<TARGET_FILE:TestComputeCommonRange> ${rightCondyle}
)

# Test 6 of UpdateColorMapByDirection in the class ShapePopulationBase
add_executable(TestUpdateColorMapByDirection mainTestUpdateColorMapByDirection.cxx testUpdateColorMapByDirection.cxx)
target_link_libraries(TestUpdateColorMapByDirection ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_UpdateColorMapByDirection
        COMMAND $<TARGET_FILE:TestUpdateColorMapByDirection> ${rightCondyle} ${rightCondyleExpectedResult}
)

# Test 7 of setBackgroundSelectedColor in the class ShapePopulationBase
add_executable(TestBackgroundSelectedColor mainTestBackgroundSelectedColor.cxx testBackgroundSelectedColor.cxx)
target_link_libraries(TestBackgroundSelectedColor ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setBackgroundSelectedColor
        COMMAND $<TARGET_FILE:TestBackgroundSelectedColor> ${rightCondyle} 
)

# Test 8 of setLabelColor in the class ShapePopulationBase
add_executable(TestLabelColor mainTestLabelColor.cxx testLabelColor.cxx)
target_link_libraries(TestLabelColor ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setLabelColor
        COMMAND $<TARGET_FILE:TestLabelColor> ${rightCondyle} 
)

# Test 9 of setBackgroundUnselectedColor in the class ShapePopulationBase
add_executable(TestBackgroundUnselectedColor mainTestBackgroundUnselectedColor.cxx testBackgroundUnselectedColor.cxx)
target_link_libraries(TestBackgroundUnselectedColor ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_setBackgroundUnselectedColor
        COMMAND $<TARGET_FILE:TestBackgroundUnselectedColor> ${rightCondyle} 
)

# Test 10 of creationSphereWidget in the class ShapePopulationBase
add_executable(TestCreationSphereWidget mainTestCreationSphereWidget.cxx testCreationSphereWidget.cxx)
target_link_libraries(TestCreationSphereWidget ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_creationSphereWidget
        COMMAND $<TARGET_FILE:TestCreationSphereWidget> ${sphere} 
)

# Test 11 of displayColorbar in the class ShapePopulationBase
add_executable(TestDisplayColorbar mainTestDisplayColorbar.cxx testDisplayColorbar.cxx)
target_link_libraries(TestDisplayColorbar ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayColorbar
        COMMAND $<TARGET_FILE:TestDisplayColorbar> ${rightCondyle} 
)

# Test 12 of displayAttribute in the class ShapePopulationBase
add_executable(TestDisplayAttribute mainTestDisplayAttribute.cxx testDisplayAttribute.cxx)
target_link_libraries(TestDisplayAttribute ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayAttribute
        COMMAND $<TARGET_FILE:TestDisplayAttribute> ${rightCondyle} 
)

# Test 13 of displayMeshName in the class ShapePopulationBase
add_executable(TestDisplayMeshName mainTestDisplayMeshName.cxx testDisplayMeshName.cxx)
target_link_libraries(TestDisplayMeshName ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayMeshName
        COMMAND $<TARGET_FILE:TestDisplayMeshName> ${rightCondyle} 
)

# Test 14 of displayVectors in the class ShapePopulationBase
add_executable(TestDisplayVectors mainTestDisplayVectors.cxx testDisplayVectors.cxx)
target_link_libraries(TestDisplayVectors ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayVectors
        COMMAND $<TARGET_FILE:TestDisplayVectors> ${rightCondyle} 
)

# Test 15 of computeCommonAttributes in the class ShapePopulationBase
add_executable(TestComputeCommonAttributes mainTestComputeCommonAttributes.cxx testComputeCommonAttributes.cxx)
target_link_libraries(TestComputeCommonAttributes ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_computeCommonAttributes
        COMMAND $<TARGET_FILE:TestComputeCommonAttributes> ${rightCondyle} ${rightCondyle2} 
)

# Test 16 of displayColorMapByMagnitude and displayColorMapByDirection in the class ShapePopulationBase
add_executable(TestDisplayColorMap mainTestDisplayColorMap.cxx testDisplayColorMap.cxx)
target_link_libraries(TestDisplayColorMap ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_displayColorMap
        COMMAND $<TARGET_FILE:TestDisplayColorMap> ${rightCondyle} ${rightCondyleExpectedResult2} 
)

# Test 17 of UpdateAttribute in the class ShapePopulationBase
add_executable(TestUpdateAttribute mainTestUpdateAttribute.cxx testUpdateAttribute.cxx)
target_link_libraries(TestUpdateAttribute ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_UpdateAttribute
        COMMAND $<TARGET_FILE:TestUpdateAttribute> ${rightCondyle} ${rightCondyleExpectedResult2} 
)

# Test 18 of UpdateColorMapByMagnitude in the class ShapePopulationBase
add_executable(TestUpdateColorMapByMagnitude mainTestUpdateColorMapByMagnitude.cxx testUpdateColorMapByMagnitude.cxx)
target_link_libraries(TestUpdateColorMapByMagnitude ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_UpdateColorMapByMagnitude
        COMMAND $<TARGET_FILE:TestUpdateColorMapByMagnitude> ${rightCondyle}  
)

# Test 19 of the color mode of vectors in the class ShapePopulationBase
add_executable(TestVectorsColorMode mainTestVectorsColorMode.cxx testVectorsColorMode.cxx)
target_link_libraries(TestVectorsColorMode ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_VectorsColorMode
        COMMAND $<TARGET_FILE:TestVectorsColorMode> ${rightCondyle}  
)

# Test 20 of getSelectedIndex in the class ShapePopulationBase
add_executable(TestSelectedIndex mainTestSelectedIndex.cxx testSelectedIndex.cxx)
target_link_libraries(TestSelectedIndex ShapePopulationViewerLib)
ExternalData_add_test(
        MY_DATA
        NAME TestShapePopulationBase_getSelectedIndex
        COMMAND $<TARGET_FILE:TestSelectedIndex> ${rightCondyle}  
)

# Test for the command --help
add_test(
        NAME PrintHelp
        COMMAND $<TARGET_FILE:ShapePopulationViewer> --help
)

ExternalData_Add_Target(MY_DATA)

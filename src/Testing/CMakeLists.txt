cmake_minimum_required(VERSION 2.8.3)

# Add external data from midas
list(APPEND ExternalData_URL_TEMPLATES
"http://slicer.kitware.com/midas3/api/rest?method=midas.bitstream.download&checksum=%(hash)"
)

# Set the data
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)
set(sphere DATA{${SOURCE_DIR}/sphereExpectedResult.vtk})
set(rightCondyle DATA{${SOURCE_DIR}/rightCondyle.vtk})

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

# Test for the command --help
add_test(
        NAME PrintHelp
        COMMAND $<TARGET_FILE:ShapePopulationViewer> --help
)

ExternalData_Add_Target(MY_DATA)

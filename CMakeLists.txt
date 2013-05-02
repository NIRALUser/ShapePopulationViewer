
cmake_minimum_required(VERSION 2.8)
 
PROJECT(ShapePopulationViewer)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}) 

file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)

QT4_WRAP_UI(UISrcs ${UI_FILES})
QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
add_executable(ShapePopulationViewer MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs})
 
if(VTK_LIBRARIES)
  if(${VTK_VERSION} VERSION_LESS "6")
    target_link_libraries(ShapePopulationViewer ${VTK_LIBRARIES} QVTK)
  else()
    target_link_libraries(ShapePopulationViewer ${VTK_LIBRARIES})
  endif()
else()
  target_link_libraries(ShapePopulationViewer vtkHybrid QVTK vtkViews ${QT_LIBRARIES})
endif()

ENABLE_TESTING()
add_executable(Testing/filePathTest Testing/filePathTest.cxx)
target_link_libraries(Testing/filePathTest ${QT_LIBRARIES} ${VTK_LIBRARIES} QVTK vtkViews vtkHybrid)
add_test(Testing/filePathTest  ${CMAKE_CURRENT_BINARY_DIR}/Testing/filePathTest)


add_executable(Testing/mapperTest Testing/mapper.cxx)
target_link_libraries(Testing/mapperTest ${QT_LIBRARIES} ${VTK_LIBRARIES} QVTK vtkViews vtkHybrid)
add_test(Testing/mapperTest  ${CMAKE_CURRENT_BINARY_DIR}/Testing/mapperTest)


add_executable(Testing/qVectorTests Testing/qVectorTests.cxx)
target_link_libraries(Testing/qVectorTests ${QT_LIBRARIES} ${VTK_LIBRARIES} QVTK vtkViews vtkHybrid)
add_test(Testing/qVectorTests  ${CMAKE_CURRENT_BINARY_DIR}/Testing/qVectorTests)


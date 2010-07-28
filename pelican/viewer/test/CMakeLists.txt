SUBPACKAGE(viewerTest viewer)
set(
   viewerTest_src
   src/viewerTest.cpp
)
add_executable(viewerTest ${viewerTest_src} )
target_link_libraries(viewerTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )
add_test(viewerTest viewerTest )

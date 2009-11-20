SUBPACKAGE(utilityTest utility)
set(
   utilityTest_src
   src/utilityTest.cpp
   src/memoryTracerTest.cpp
   src/ConfigTest.cpp
)
add_executable(utilityTest ${utilityTest_src} )
target_link_libraries(utilityTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES})
add_test(utilityTest ${EXECUTABLE_OUTPUT_PATH}/utilityTest )

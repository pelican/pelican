include(CopyFiles)
SUBPACKAGE(utilityTest utility)
set(
   utilityTest_src
   src/utilityTest.cpp
   src/memoryTracerTest.cpp
   src/ConfigTest.cpp
)
add_executable(utilityTest ${utilityTest_src} )
target_link_libraries(utilityTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)
add_test(utilityTest ${EXECUTABLE_OUTPUT_PATH}/utilityTest )

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/*.xml .)




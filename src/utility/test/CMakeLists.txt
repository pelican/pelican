include(CopyFiles)
SUBPACKAGE(utilityTest utility)
set(
   utilityTest_src
   src/utilityTest.cpp
   src/SocketTesterTest.cpp
   src/ConfigTest.cpp
   src/ConfigNodeTest.cpp
)

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    list(APPEND utilityTest_src src/memoryTracerTest.cpp)
endif (CMAKE_BUILD_TYPE MATCHES DEBUG)

add_executable(utilityTest ${utilityTest_src} )
target_link_libraries(utilityTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)
add_test(utilityTest utilityTest )

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/*.xml .)




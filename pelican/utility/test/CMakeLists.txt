# Set dependencies for Pelican utility tests.
SUBPACKAGE(utilityTest utility comms server)

# Build Pelcain utility tests.
# Note: The memory tracer utility is only availaible for linking in debug mode.
# ==============================================================================
set(utilityTest_src
    src/utilityTest.cpp
    src/SocketTesterTest.cpp
    src/ConfigTest.cpp
    src/ConfigNodeTest.cpp
)
#message(STATUS ">>>>>>>>>>>>>> utilityTest libs : ${SUBPACKAGE_LIBRARIES}")

if(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
    list(APPEND utilityTest_src src/memoryTracerTest.cpp)
    add_executable(utilityTest ${utilityTest_src} )
    target_link_libraries(utilityTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
        memoryTracer
    )
else(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
    add_executable(utilityTest ${utilityTest_src} )
    target_link_libraries(utilityTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )
endif(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
add_test(utilityTest utilityTest)

# Copy test xml files to build directory.
include(CopyFiles)
copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/*.xml . testXMLFiles)

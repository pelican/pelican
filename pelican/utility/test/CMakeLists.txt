# Build the Pelican utility testing library module
# ==============================================================================
SUBPACKAGE(utilityTest utility)
set(utilityTestLib_src
    src/SocketTester.cpp
    src/TestFile.cpp
)
SUBPACKAGE_LIBRARY(utilityTestUtils ${utilityTestLib_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)


# Build Pelcain utility tests.
# Note: The memory tracer utility is only availaible for linking in debug mode.
# ==============================================================================
set(utilityTest_src
    src/utilityTest.cpp
    src/SocketTesterTest.cpp
    src/ConfigTest.cpp
    src/ConfigNodeTest.cpp
    src/PelicanTimeRecorderTest.cpp
)
set(utilityTest_mt_src
    src/utilityTest.cpp
    src/WatchedFileTest.cpp
)

add_executable(utilityTestMT ${utilityTest_mt_src} )
if(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
    list(APPEND utilityTest_src src/memoryTracerTest.cpp)
    add_executable(utilityTest ${utilityTest_src} )
    target_link_libraries(utilityTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
        memoryTracer
    )
    target_link_libraries(utilityTestMT
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
        memoryTracerDummy
    )
else(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
    add_executable(utilityTest ${utilityTest_src} )
    target_link_libraries(utilityTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )
    target_link_libraries(utilityTestMT
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
        )
endif(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
add_test(utilityTest utilityTest)
add_test(utilityTestMT utilityTestMT)

# Copy test xml files to build directory.
include(CopyFiles)
copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/*.xml . testXMLFiles)

# Set dependencies for Pelican core tests.
TEST_SUBPACKAGE(coreTest core data utility serverTest emulator)

# Build Pelican core testing library module.
# ==============================================================================
set(coreTestLibrary_src
    src/coreTest.cpp
    src/TestPipeline.cpp
    src/TestDataClient.cpp
    src/TestAdapter.cpp
    src/TestStreamAdapter.cpp
)
TEST_SUBPACKAGE_LIBRARY(coreTestUtilities ${coreTestLibrary_src})
SUBPACKAGE_ADD_LIBRARIES(coreTestUtilities)


# Build single-threaded Pelcain core tests.
# ==============================================================================
set(coreTest_src
    src/coreTest.cpp
    src/AdapterRealDataTest.cpp
    src/DataTypesTest.cpp
    src/ModuleFactoryTest.cpp
    src/PipelineApplicationTest.cpp
    src/PipelineDriverTest.cpp
    src/PelicanServerClientTest.cpp
)

add_executable(coreTest ${coreTest_src})
target_link_libraries(coreTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)
add_test(coreTest coreTest)



# Build multi-threaded threaded Pelcain core tests.
# ==============================================================================
set(coreTestMT_src
    src/PelicanServerClientTestMT.cpp
    src/DirectStreamDataClientTest.cpp
)

add_executable(coreTestMT ${coreTestMT_src})
target_link_libraries(coreTestMT
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracerDummy
)
add_test(coreTestMT coreTestMT)



# Build test direct streaming client (used for bandwidth testing) 
# ==============================================================================
add_executable(directClient src/directClientMain.cpp)
target_link_libraries(directClient
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracerDummy
)



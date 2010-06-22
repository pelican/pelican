
# Build Pelican core testing library module.
# ==============================================================================
SUBPACKAGE(coreTestUtils core data utility serverTestUtils emulator)
set(coreTestLibrary_src
    src/TestPipeline.cpp
    src/TestDataClient.cpp
    src/TestAdapter.cpp
    src/TestStreamAdapter.cpp
)
SUBPACKAGE_LIBRARY(coreTestUtils ${coreTestLibrary_src})


# Build single-threaded Pelcain core tests.
# ==============================================================================
USE_SUBPACKAGES(core data utility serverTestUtils emulator coreTestUtils)
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
    src/coreTest.cpp
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
    memoryTracerDummy
)



#
# pelican/core/test/CMakeLists.txt
#

set(module pelican_coreTestUtils)
set(${module}_src
    src/TestPipeline.cpp
    src/TestDataClient.cpp
    src/TestServiceAdapter.cpp
    src/TestStreamAdapter.cpp
    src/AdapterTester.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_core
         pelican_data
         pelican_utility
         pelican_emulator
         pelican_serverTestUtils
         pelican_dataTestUtils
         pelican_utilityTestUtils
    LIBS ${QT_QTCORE_LIBRARY}
         ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# Build test direct streaming client (used for bandwidth testing)
add_executable(directClient src/directClientMain.cpp)
target_link_libraries(directClient ${${module}_LIBRARY})

if (CPPUNIT_FOUND)
    # Build single-threaded Pelcain core tests.
    set(coreTest_src
        src/CppUnitMain.cpp
        src/FileDataClientTest.cpp
        src/AdapterRealDataTest.cpp
        src/DataTypesTest.cpp
        src/ModuleFactoryTest.cpp
        src/PipelineApplicationTest.cpp
        src/PipelineDriverTest.cpp
        src/PelicanServerClientTest.cpp
        src/AbstractPipelineTest.cpp
    )
    add_executable(coreTest ${coreTest_src})
    target_link_libraries(coreTest 
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES})
    add_test(coreTest coreTest)

    # Build multi-threaded threaded Pelcain core tests.
    set(coreTestMT_src
        src/CppUnitMain.cpp
        src/PelicanServerClientTestMT.cpp
        src/DirectStreamDataClientTest.cpp
    )
    add_executable(coreTestMT ${coreTestMT_src})
    target_link_libraries(coreTestMT 
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES})
    add_test(coreTestMT coreTestMT)

endif (CPPUNIT_FOUND)



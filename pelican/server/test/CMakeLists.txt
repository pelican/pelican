# Set dependencies for Pelican server tests.
TEST_SUBPACKAGE(serverTest server emulator)

# Build Pelican server testing library module.
# ==============================================================================
set(serverTestlib_src
    src/TestChunker.cpp
    src/TestUdpChunker.cpp
    src/TestProtocol.cpp
    src/TestServer.cpp
    src/PelicanTestClient.cpp
)
set(serverTestLib_moc_sources
    TestServer.h
    TestChunker.h
)
QT4_WRAP_CPP(serverTestLib_moc_headers ${serverTestLib_moc_sources})
TEST_SUBPACKAGE_LIBRARY(serverTestUtilities
    ${serverTestlib_src}
    ${serverTestLib_moc_headers}
)
SUBPACKAGE_ADD_LIBRARIES(serverTestUtilities)


# Build single-threaded Pelcain server tests.
# ==============================================================================
set(serverTest_src
    src/serverTest.cpp
    src/ChunkerFactoryTest.cpp
    src/LockableStreamDataTest.cpp
    src/LockedDataTest.cpp
    src/DataManagerTest.cpp
    src/ServiceDataBufferTest.cpp
    src/StreamDataBufferTest.cpp
    src/SessionTest.cpp
    src/WritableDataTest.cpp
)
add_executable(serverTest ${serverTest_src} )
target_link_libraries(serverTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)
add_test(serverTest serverTest)


# Build single-threaded Pelcain server tests.
# ==============================================================================
set(serverTestMT_src
    src/serverTest.cpp
    src/PelicanServerTest.cpp
    src/DataReceiverTest.cpp
)
add_executable(serverTestMT ${serverTestMT_src})
target_link_libraries(serverTestMT
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracerDummy
)
add_test(serverTestMT serverTestMT)

SUBPACKAGE(serverTest server)
set(serverTestlib_src
    src/TestChunker.cpp
    src/TestUdpChunker.cpp
    src/TestProtocol.cpp
    src/TestServer.cpp
    src/PelicanTestClient.cpp
    src/TelescopeEmulator.cpp
)

set(serverTestLib_moc_sources
    TestServer.h
    TestChunker.h
)

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

QT4_WRAP_CPP(
    serverTestLib_moc_headers
    ${serverTestLib_moc_sources} )

# multi-threaded tests
# these need to be seperate so as not to use the
# single threaded utility libs
set(serverTestMT_src
    src/serverTest.cpp
    src/PelicanServerTest.cpp
    src/DataReceiverTest.cpp
)

add_library( serverTestUtilities ${serverTestlib_src} ${serverTestLib_moc_headers} )
SUBPACKAGE_ADD_LIBRARIES(serverTestUtilities)

add_executable(serverTest ${serverTest_src} )
target_link_libraries(serverTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )

add_executable(serverTestMT ${serverTestMT_src} )
target_link_libraries(serverTestMT ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracerDummy )

add_test(serverTest serverTest )
add_test(serverTestMT serverTestMT )

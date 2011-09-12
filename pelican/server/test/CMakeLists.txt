#
# pelican/server/test/CMakeLists.txt
#

# Build Pelican server testing library module.
include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(serverTest server emulator data comms utility emulatorTest)
set(serverTestlib_src
    src/TestChunker.cpp
    src/TestUdpChunker.cpp
    src/TestProtocol.cpp
    src/TestServer.cpp
    src/PelicanTestClient.cpp
    src/ChunkerTester.cpp
)
set(serverTestLib_moc_sources
    TestServer.h
    TestChunker.h
)
QT4_WRAP_CPP(serverTestlib_src ${serverTestLib_moc_sources})
SUBPACKAGE_LIBRARY(serverTestUtils ${serverTestlib_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
)


if (CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    # Build single-threaded Pelcain server tests.
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
    add_executable(serverTest ${serverTest_src})
    target_link_libraries(serverTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(serverTest serverTest)


    # Build single-threaded Pelcain server tests.
    set(serverTestMT_src
        src/serverTest.cpp
        src/PelicanServerTest.cpp
        src/DataReceiverTest.cpp
        src/FileChunkerTest.cpp
    )
    add_executable(serverTestMT ${serverTestMT_src})
    target_link_libraries(serverTestMT
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(serverTestMT serverTestMT)
endif (CPPUNIT_FOUND)

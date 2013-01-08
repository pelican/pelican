#
# pelican/server/test/CMakeLists.txt
#

# Build Pelican server testing library module.
set(module pelican_serverTestUtils)
set(${module}_src
    src/TestChunker.cpp
    src/TestUdpChunker.cpp
    src/TestProtocol.cpp
    src/TestServer.cpp
    src/PelicanTestClient.cpp
    src/ChunkerTester.cpp
)
set(${module}_moc
    TestServer.h
    TestChunker.h
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc}
    DEPS pelican_server
         pelican_emulator
         pelican_data
         pelican_comms
         pelican_utility
         pelican_emulatorTestUtils
    LIBS ${QT_QTCORE_LIBRARY}
)

if (CPPUNIT_FOUND)
    # Build single-threaded Pelcain server tests.
    set(serverTest_src
        src/CppUnitMain.cpp
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
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(serverTest serverTest)

    # Build single-threaded Pelcain server tests.
    set(serverTestMT_src
        src/CppUnitMain.cpp
        src/PelicanServerTest.cpp
        src/DataReceiverTest.cpp
        src/FileChunkerTest.cpp
    )
    add_executable(serverTestMT ${serverTestMT_src})
    target_link_libraries(serverTestMT
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(serverTestMT serverTestMT)
endif (CPPUNIT_FOUND)

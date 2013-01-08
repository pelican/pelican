#
# pelican/output/test/CMakeLists.txt
#


# Build Pelican output test library.
set(module pelican_outputTestUtils)
set(${module}_src
    src/TestOutputStreamer.cpp
    src/TestDataBlobClient.cpp
    src/TestDataBlobServer.cpp
)
set(${module}_moc
    TestDataBlobClient.h
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc}
    DEPS pelican_output 
         pelican_dataTestUtils
         pelican_serverTestUtils
         pelican_utilityTestUtils
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY}
)

if (CPPUNIT_FOUND)
    set(name outputTestMT)
    set(${name}_src
        src/CppUnitMain.cpp
        src/TCPConnectionManagerTest.cpp
        src/PelicanTCPBlobServerTest.cpp
        src/OutputStreamManagerTest.cpp
        src/DataBlobClientTest.cpp
        src/ThreadedDataBlobClientTest.cpp
        src/DataBlobFileTest.cpp
        src/DataBlobChunkerTest.cpp
        src/DataBlobRelayTest.cpp
    )
    add_executable(${name} ${${name}_src})
    target_link_libraries(${name}
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(${name} ${name})
endif (CPPUNIT_FOUND)

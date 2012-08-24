#
# pelican/output/test/CMakeLists.txt
#

# Set dependencies for Pelican output tests.
include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(outputTest output dataTest serverTest utilityTest)

# Build Pelican output test library.
# ==============================================================================
set(outputTestUtils_src
    src/TestOutputStreamer.cpp
    src/TestDataBlobClient.cpp
    src/TestDataBlobServer.cpp
)
set(outputTestUtils_moc_headers
    TestDataBlobClient.h
)

QT4_WRAP_CPP(outputTestUtils_src ${outputTestUtils_moc_headers})
SUBPACKAGE_LIBRARY(outputTestUtils ${outputTestUtils_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
)



if (CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    # Build Pelican output tests.
    # ==============================================================================
    set(outputTest_src
        src/outputTest.cpp
        src/TCPConnectionManagerTest.cpp
        src/PelicanTCPBlobServerTest.cpp
        src/OutputStreamManagerTest.cpp
        src/DataBlobClientTest.cpp
        src/ThreadedDataBlobClientTest.cpp
        src/DataBlobFileTest.cpp
        src/DataBlobChunkerTest.cpp
        src/DataBlobRelayTest.cpp
    )
    add_executable(outputTestMT ${outputTest_src})
    target_link_libraries(outputTestMT
        outputTestUtils
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(outputTestMT outputTestMT)
endif (CPPUNIT_FOUND)

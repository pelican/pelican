# Set dependencies for Pelican output tests.
SUBPACKAGE(outputTest output dataTest)

# Build Pelican output test library.
# ==============================================================================
set(outputTestUtils_src
    src/TestOutputStreamer.cpp
    src/TestDataBlobClient.cpp
)
set( outputTestUtils_moc_headers
     TestDataBlobClient.h
   )

QT4_WRAP_CPP(outputTestUtils_src ${outputTestUtils_moc_headers})
SUBPACKAGE_LIBRARY(outputTestUtils ${outputTestUtils_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Build Pelican output tests.
# ==============================================================================
set(outputTest_src
    src/outputTest.cpp
    src/TCPConnectionManagerTest.cpp
    src/PelicanTCPBlobServerTest.cpp
    src/OutputStreamManagerTest.cpp
    src/DataBlobClientTest.cpp
    src/ThreadedDataBlobClientTest.cpp
)
add_executable(outputTestMT ${outputTest_src})
target_link_libraries(outputTestMT
    outputTestUtils
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracerDummy
)
add_test(outputTestMT outputTestMT)


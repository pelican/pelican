# Set dependencies for Pelican output tests.
SUBPACKAGE(outputTest output dataTest)

# Build Pelican output test library.
# ==============================================================================
set(outputTestUtils_src
    src/TestOutputStreamer.cpp
    )

SUBPACKAGE_LIBRARY(outputTestUtils ${outputTestUtils_src})

# Build Pelican output tests.
# ==============================================================================
set(outputTest_src
    src/outputTest.cpp
    src/TCPConnectionManagerTest.cpp
    src/PelicanTCPBlobServerTest.cpp
    src/OutputStreamManagerTest.cpp
)
add_executable(outputTestMT ${outputTest_src})
target_link_libraries(outputTestMT 
    ${SUBPACKAGE_LIBRARIES} 
    ${CPPUNIT_LIBRARIES} 
    memoryTracerDummy 
)
add_test(outputTestMT outputTestMT)


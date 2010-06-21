# Set dependencies for Pelican output tests.
USE_SUBPACKAGES(output dataTestUtils)

# Build Pelican output tests.
# ==============================================================================
set(outputTest_src
    src/outputTest.cpp
    src/TCPConnectionManagerTest.cpp
    src/PelicanTCPBlobServerTest.cpp
)
add_executable(outputTestMT ${outputTest_src})
target_link_libraries(outputTestMT 
    ${SUBPACKAGE_LIBRARIES} 
    ${CPPUNIT_LIBRARIES} 
    memoryTracerDummy 
)
add_test(outputTestMT outputTestMT)


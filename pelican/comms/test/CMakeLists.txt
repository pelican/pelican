# Build Pelican comms tests.
# ==============================================================================
SUBPACKAGE(commsTest comms dataTest utilityTest)
set(commsTest_src
    src/commsTest.cpp
    src/DataChunkTest.cpp
    src/StreamDataTest.cpp
    src/PelicanProtocolTest.cpp
)
add_executable(commsTest ${commsTest_src})

target_link_libraries(commsTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)
add_test(commsTest commsTest)


# Build Pelican emulator tests.
# ==============================================================================
SUBPACKAGE(emulatorTest emulator utility)
set(emulatorTest_src
    src/emulatorTest.cpp
)
add_executable(emulatorTest ${emulatorTest_src})
target_link_libraries(emulatorTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)
add_test(emulatorTest emulatorTest)

# Build emulator binary to send UDP packets containing real, floating point
# values (used for bandwith testing). 
# ==============================================================================
add_executable(realUdpEmulator src/realUdpEmulator.cpp)
target_link_libraries(realUdpEmulator
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracerDummy
)


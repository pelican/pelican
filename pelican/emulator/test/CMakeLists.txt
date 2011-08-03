# Build Pelican emulator testing library module.
# ==============================================================================
SUBPACKAGE(emulatorTest emulator utility)
set(emulatorTestLib_src
    src/RealUdpEmulator.cpp
)
SUBPACKAGE_LIBRARY(emulatorTestUtils ${emulatorTestLib_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
)



# Build test binary.
#===============================================================================
set(emulatorTest_src
    src/emulatorTest.cpp
)
add_executable(emulatorTest ${emulatorTest_src})
target_link_libraries(emulatorTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
)
add_test(emulatorTest emulatorTest)



# Build emulator binary to send UDP packets containing real, floating point
# values (used for bandwith testing).
# ==============================================================================
add_executable(realUdpEmulator src/realUdpEmulatorMain.cpp)
target_link_libraries(realUdpEmulator
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
)


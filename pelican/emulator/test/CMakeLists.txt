#
# pelican/emulator/test/CMakeLists.txt
#

# Build Pelican emulator testing library module.

set(module pelican_emulatorTestUtils)
declare_module_library(${module}
    SRC  src/RealUdpEmulator.cpp
    LIBS ${QT_QTCORE_LIBRARY}
    DEPS pelican_emulator pelican_utility
)


if (CPPUNIT_FOUND)
    add_executable(emulatorTest 
        src/CppUnitMain.cpp
    )
    target_link_libraries(emulatorTest
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(emulatorTest emulatorTest)

    # Build emulator binary to send UDP packets containing real, floating point
    # values (used for bandwith testing).
    # ==============================================================================
    add_executable(realUdpEmulator src/realUdpEmulatorMain.cpp)
    target_link_libraries(realUdpEmulator
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )
endif (CPPUNIT_FOUND)

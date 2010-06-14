SUBPACKAGE(emulatorTest emulator)

set(emulatorTest_src
    src/emulatorTest.cpp
)

add_executable(emulatorTest ${emulatorTest_src} )
target_link_libraries(emulatorTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)

add_executable(realUdpEmulator src/realUdpEmulator.cpp)
target_link_libraries(realUdpEmulator
    ${SUBPACKAGE_LIBRARIES}
    memoryTracerDummy
)

add_test(emulatorTest emulatorTest)

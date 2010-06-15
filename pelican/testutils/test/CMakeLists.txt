SUBPACKAGE(testutilsTest testutils)
set(testutilsTest_src
    src/testutilsTest.cpp
)

add_executable(testutilsTest ${testutilsTest_src} )
target_link_libraries(testutilsTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )

add_executable(realUdpEmulator src/realUdpEmulator.cpp )
target_link_libraries(realUdpEmulator ${SUBPACKAGE_LIBRARIES} memoryTracerDummy)

add_test(testutilsTest testutilsTest )

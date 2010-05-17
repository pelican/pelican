SUBPACKAGE(testutilsTest testutils)
set(
   testutilsTest_src
src/testutilsTest.cpp
)
add_executable(testutilsTest ${testutilsTest_src} )
target_link_libraries(testutilsTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )
add_test(testutilsTest testutilsTest )

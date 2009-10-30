SUBPACKAGE(serverTest server)
set(
   serverTest_src
src/serverTest.cpp
)
add_executable(serverTest ${serverTest_src} )
target_link_libraries(serverTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(serverTest ${EXECUTABLE_OUTPUT_PATH}/serverTest )

SUBPACKAGE(serverTest server)
set(
   serverTestlib_src
   src/TestProtocol.cpp
   src/PelicanTestClient.cpp
   )
set(
   serverTest_src
   src/serverTest.cpp
   src/DataTest.cpp
   src/DataManagerTest.cpp
   src/LockedDataTest.cpp
   src/SessionTest.cpp
   src/ServiceDataBufferTest.cpp
   src/StreamDataBufferTest.cpp
   src/StreamDataTest.cpp
   src/WritableDataTest.cpp
   #   src/PelicanServerTest.cpp
)

add_library( serverTestUtilities ${serverTestlib_src} )
SUBPACKAGE_ADD_LIBRARIES(serverTestUtilities)

add_executable(serverTest ${serverTest_src} )
target_link_libraries(serverTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(serverTest ${EXECUTABLE_OUTPUT_PATH}/serverTest )

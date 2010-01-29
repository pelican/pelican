SUBPACKAGE(commsTest comms)
set(
   commsTest_src
   src/commsTest.cpp
   #src/PelicanProtocolTest.cpp
)
add_executable(commsTest ${commsTest_src} )
target_link_libraries(commsTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )
add_test(commsTest ${EXECUTABLE_OUTPUT_PATH}/commsTest )

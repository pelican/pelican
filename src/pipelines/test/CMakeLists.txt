SUBPACKAGE(pipelinesTest pipelines)
set(
   pipelinesTest_src
src/pipelinesTest.cpp
)
add_executable(pipelinesTest ${pipelinesTest_src} )
target_link_libraries(pipelinesTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(pipelinesTest ${EXECUTABLE_OUTPUT_PATH}/pipelinesTest )

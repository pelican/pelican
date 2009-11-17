SUBPACKAGE(coreTest core)
set( coreTest_src
     src/coreTest.cpp
     src/PipelineDriverTest.cpp
)
add_executable(coreTest ${coreTest_src} )
target_link_libraries(coreTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(coreTest ${EXECUTABLE_OUTPUT_PATH}/coreTest )

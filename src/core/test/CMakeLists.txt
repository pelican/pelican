SUBPACKAGE(coreTest core data utility)
set( coreTest_src
     src/coreTest.cpp
     src/ModuleFactoryTest.cpp
     src/PipelineApplicationTest.cpp
     src/PipelineDriverTest.cpp
)
set( coreTestLibrary_src
     src/TestDataClient.cpp
     src/TestModule.cpp
     src/TestPipeline.cpp
)
add_library(coreTestUtilities ${coreTestLibrary_src} )
SUBPACKAGE_ADD_LIBRARIES(coreTestUtilities)
add_executable(coreTest ${coreTest_src} )
target_link_libraries(coreTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES})
add_test(coreTest ${EXECUTABLE_OUTPUT_PATH}/coreTest )



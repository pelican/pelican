SUBPACKAGE(coreTest core data utility adaptersTest serverTest)
set( coreTest_src
     src/coreTest.cpp
     src/DataTypesTest.cpp
     #src/ModuleFactoryTest.cpp
     src/PipelineApplicationTest.cpp
     #src/PipelineDriverTest.cpp
     src/PelicanServerClientTest.cpp
)
set( coreTestMT_src
     src/PelicanServerClientTestMT.cpp
   )

set( coreTestLibrary_src
     src/coreTest.cpp
     src/TestPipeline.cpp
)
add_library(coreTestUtilities ${coreTestLibrary_src} )
SUBPACKAGE_ADD_LIBRARIES(coreTestUtilities)

add_executable(coreTest ${coreTest_src} )
target_link_libraries(coreTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )
add_executable(coreTestMT ${coreTestMT_src} )
target_link_libraries(coreTestMT ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracerDummy )

add_test(coreTest coreTest )

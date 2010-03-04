SUBPACKAGE(adaptersTest adapters utility)

set( adaptersTest_src
     src/adaptersTest.cpp
     src/AdapterLofarStationVisibilitiesTest.cpp
     src/AdapterAntennaListTest.cpp
)

set( adaptersTestLibrary_src
     src/VisGen.cpp
     src/TestDataBlob.cpp
     src/TestAdapter.cpp
     src/TestAdapterFactory.cpp
)
add_library(adaptersTestUtilities ${adaptersTestLibrary_src} )
SUBPACKAGE_ADD_LIBRARIES(adaptersTestUtilities)

add_executable(adaptersTest ${adaptersTest_src} )
target_link_libraries(adaptersTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )

add_executable(testVisGen src/testVisGen.cpp )
target_link_libraries(testVisGen ${SUBPACKAGE_LIBRARIES} memoryTracer )

add_test(adaptersTest adaptersTest)

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/* . adaptersTestData)

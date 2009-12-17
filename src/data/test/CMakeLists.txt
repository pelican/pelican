SUBPACKAGE(dataTest data)
set(
   dataTest_src
   src/dataTest.cpp
   src/DataRequirementsTest.cpp
)
add_executable(dataTest ${dataTest_src} )
target_link_libraries(dataTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(dataTest ${EXECUTABLE_OUTPUT_PATH}/dataTest )

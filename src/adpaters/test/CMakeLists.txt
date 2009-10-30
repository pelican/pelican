SUBPACKAGE(adpatersTest adpaters)
set(
   adpatersTest_src
src/adpatersTest.cpp
)
add_executable(adpatersTest ${adpatersTest_src} )
target_link_libraries(adpatersTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(adpatersTest ${EXECUTABLE_OUTPUT_PATH}/adpatersTest )

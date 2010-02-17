SUBPACKAGE(adaptersTest adapters utility)

set( adaptersTest_src
     src/adaptersTest.cpp
)

add_executable(adaptersTest ${adaptersTest_src} )
target_link_libraries(adaptersTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )
add_test(adaptersTest adaptersTest memoryTracer )

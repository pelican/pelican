SUBPACKAGE(outputTest output)

set(outputTest_src
    src/outputTest.cpp
    src/PelicanTCPBlobServerTest.cpp
)

add_executable(outputTestMT ${outputTest_src} )

target_link_libraries(outputTestMT 
    ${SUBPACKAGE_LIBRARIES} 
    ${CPPUNIT_LIBRARIES} 
    memoryTracerDummy 
)

add_test(outputTestMT outputTestMT)

target_link_libraries(outputTestMT ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracerDummy )

#
# pelican/comms/test/CMakeLists.txt
#

if (CPPUNIT_FOUND)

    include_directories(${CPPUNIT_INCLUDE_DIR})
    SUBPACKAGE(commsTest comms dataTest utilityTest)

    set(commsTest_src
        src/commsTest.cpp
        src/DataChunkTest.cpp
        src/StreamDataTest.cpp
        src/PelicanProtocolTest.cpp
    )
    add_executable(commsTest ${commsTest_src})
    target_link_libraries(commsTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES})
    add_test(commsTest commsTest)

endif (CPPUNIT_FOUND)

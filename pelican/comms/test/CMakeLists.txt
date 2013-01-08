#
# pelican/comms/test/CMakeLists.txt
#

if (CPPUNIT_FOUND)
    set(name commsTest)
    set(${name}_src
        src/CppUnitMain.cpp
        src/DataChunkTest.cpp
        src/StreamDataTest.cpp
        src/PelicanProtocolTest.cpp
    )
    add_executable(${name} ${${name}_src})
    target_link_libraries(${name}
        ${pelican_comms_LIBRARY}
        ${pelican_dataTestUtils_LIBRARY}
        ${pelican_utilityTestUtils_LIBRARY}
        ${CPPUNIT_LIBRARIES})
    add_test(${name} ${name})
endif (CPPUNIT_FOUND)

SUBPACKAGE(dataTest data)

set(dataTest_src
    src/dataTest.cpp
    src/DataRequirementsTest.cpp
    src/ImageDataTest.cpp
    src/VisibilityDataTest.cpp
    src/FlagTableTest.cpp
    src/VisibilityPositionsTest.cpp
    src/AntennaPositionsTest.cpp
    src/TimeStreamDataTest.cpp
)

add_executable(dataTest ${dataTest_src})

target_link_libraries(dataTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)

add_test(dataTest dataTest )

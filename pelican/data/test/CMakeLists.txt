SUBPACKAGE(dataTest data)

set(dataTest_src
    src/dataTest.cpp
    src/DataRequirementsTest.cpp
)

set(dataTestLib_src
    src/TestDataBlob.cpp
)

add_executable(dataTest ${dataTest_src})


add_library(dataTestLib ${dataTestLib_src})

SUBPACKAGE_ADD_LIBRARIES(dataTestLib)

target_link_libraries(dataTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)

add_test(dataTest dataTest )

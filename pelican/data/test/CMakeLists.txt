# Build Pelican data testing library module.
# ==============================================================================
SUBPACKAGE(dataTest data)
set(dataTestLib_src
    src/TestDataBlob.cpp
)

SUBPACKAGE_LIBRARY(dataTestUtils ${dataTestLib_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
)


# Build Pelcain data tests.
# ==============================================================================
set(dataTest_src
    src/dataTest.cpp
    src/DataRequirementsTest.cpp
)
add_executable(dataTest ${dataTest_src})
#message(STATUS ">>>>>>>> dataTest: subpackage libs = ${SUBPACKAGE_LIBRARIES}")
target_link_libraries(dataTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)

add_test(dataTest dataTest)

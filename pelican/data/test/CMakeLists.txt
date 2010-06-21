# Build Pelican data testing library module.
# ==============================================================================
SUBPACKAGE(dataTestUtils data)
set(dataTestLib_src
    src/TestDataBlob.cpp
)
SUBPACKAGE_LIBRARY(dataTestUtils ${dataTestLib_src})


# Build Pelcain data tests.
# ==============================================================================
USE_SUBPACKAGES(dataTestUtils)
set(dataTest_src
    src/dataTest.cpp
    src/DataRequirementsTest.cpp
)
add_executable(dataTest ${dataTest_src})
target_link_libraries(dataTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)

add_test(dataTest dataTest)

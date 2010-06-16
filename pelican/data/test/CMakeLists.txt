# Set dependencies for Pelican data tests.
TEST_SUBPACKAGE(dataTest data)


# Build Pelican data testing library module.
# ==============================================================================
set(dataTestLib_src
    src/TestDataBlob.cpp
)
TEST_SUBPACKAGE_LIBRARY(dataTestLib ${dataTestLib_src})
SUBPACKAGE_ADD_LIBRARIES(dataTestLib) ## HACK remove soon.

# Build Pelcain data tests.
# ==============================================================================
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

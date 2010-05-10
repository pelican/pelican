SUBPACKAGE(adaptersTest adapters utility data)

set(adaptersTest_src
    src/adaptersTest.cpp
    src/AdapterLofarStationVisibilitiesTest.cpp
    src/AdapterAntennaListTest.cpp
    src/AdapterRealDataTest.cpp
)

set(adaptersTestLibrary_src
    src/VisGen.cpp
    src/TestAdapter.cpp
    src/TestStreamAdapter.cpp
)

add_library(adaptersTestUtilities ${adaptersTestLibrary_src} )

SUBPACKAGE_ADD_LIBRARIES(
    adaptersTestUtilities
)


# Create the test executable.
add_executable(adaptersTest ${adaptersTest_src})
target_link_libraries(adaptersTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)

# Create the visibility generator.
add_executable(testVisGen src/testVisGen.cpp)
target_link_libraries(testVisGen
    ${SUBPACKAGE_LIBRARIES}
    memoryTracer
)

# Add to the adapters test to the test target.
add_test(adaptersTest adaptersTest)

# Copy the test/data folder to the build tree.
copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/* . adaptersTestData)

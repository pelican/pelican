# Set dependencies for Pelican modules tests.
USE_SUBPACKAGES(modules utility)

# Build Pelican module tests.
# ==============================================================================
set(modulesTest_src
    src/modulesTest.cpp
)
add_executable(modulesTest ${modulesTest_src})
target_link_libraries(modulesTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
    memoryTracer
)
add_test(modulesTest modulesTest )

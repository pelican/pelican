SUBPACKAGE(modulesTest modules utility)

set(modulesTest_src
    src/modulesTest.cpp
    src/AstrometryFastTest.cpp
    src/BasicFlaggerTest.cpp
    #src/ZenithCalibraterTest.cpp
    src/ZenithImagerDftTest.cpp
    src/ZenithModelVisibilitiesTest.cpp
    src/ImageWriterFitsTest.cpp
)

# Create the modulesTest binary.
add_executable(modulesTest ${modulesTest_src})

# Compile with openmp as this is required by modules.
#set_target_properties(modulesTest PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}")

target_link_libraries(modulesTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )

add_test(modulesTest modulesTest )

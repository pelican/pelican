SUBPACKAGE(modulesTest modules utility)
set(
   modulesTest_src
   src/modulesTest.cpp
   src/TestModule.cpp
   src/BasicFlaggerTest.cpp
   src/ZenithCalibraterTest.cpp
   src/ZenithImagerDftTest.cpp
   src/ZenithModelVisibilitiesTest.cpp
   src/ImagerDftTest.cpp
   src/ImagerFftTest.cpp
   src/ImageWriterFitsTest.cpp
)
add_executable(modulesTest ${modulesTest_src} )
target_link_libraries(modulesTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )
add_test(modulesTest ${EXECUTABLE_OUTPUT_PATH}/modulesTest )

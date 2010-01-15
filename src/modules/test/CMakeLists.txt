SUBPACKAGE(modulesTest modules)
set(
   modulesTest_src
   src/modulesTest.cpp
   src/BasicFlaggerTest.cpp
   src/ZenithCalibraterTest.cpp
   src/ZenithImagerDftTest.cpp
   src/ZenithModelVisibilitiesTest.cpp
)
add_executable(modulesTest ${modulesTest_src} )
target_link_libraries(modulesTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} )
add_test(modulesTest ${EXECUTABLE_OUTPUT_PATH}/modulesTest )

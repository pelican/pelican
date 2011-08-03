#
# viewer/test/CMakeLists.txt
#

SUBPACKAGE(viewerTest viewer outputTest)

set(viewerTest_src
    src/viewerTest.cpp
    src/DataViewerTest.cpp
)

set(viewerUtilityTest_src
    src/TestDataViewerWidget.cpp
)

add_executable(viewerTest ${viewerTest_src} )

SUBPACKAGE_LIBRARY(viewerTestUtils ${viewerUtilityTest_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(${QT_QTCORE_LIBRARY})

target_link_libraries(viewerTest
    ${SUBPACKAGE_LIBRARIES}
    ${CPPUNIT_LIBRARIES}
)

add_test(viewerTest viewerTest)

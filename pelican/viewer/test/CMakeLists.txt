#
# viewer/test/CMakeLists.txt
#


include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(viewerTest viewer outputTest)
set(viewerUtilityTest_src
    src/TestDataViewerWidget.cpp
)
SUBPACKAGE_LIBRARY(viewerTestUtils ${viewerUtilityTest_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(${QT_QTCORE_LIBRARY})

if (CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    set(viewerTest_src
        src/viewerTest.cpp
        src/DataViewerTest.cpp
    )
    add_executable(viewerTest ${viewerTest_src} )
    target_link_libraries(viewerTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(viewerTest viewerTest)
endif (CPPUNIT_FOUND)

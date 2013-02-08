#
# viewer/test/CMakeLists.txt
#

set(module pelican_viewerTestUtils)
set(${module}_src
    src/TestDataViewerWidget.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_viewer pelican_outputTestUtils
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
)

if (CPPUNIT_FOUND)
    set(viewerTest_src
        src/CppUnitMain.cpp
        src/DataViewerTest.cpp
    )
    add_executable(viewerTest ${viewerTest_src} )
    target_link_libraries(viewerTest
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )
    add_test(viewerTest viewerTest)
endif (CPPUNIT_FOUND)

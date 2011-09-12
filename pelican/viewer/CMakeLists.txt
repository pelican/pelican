#
# pelican/viewer/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(viewer output comms utility)
set(viewer_src
    src/DataBlobWidgetFactory.cpp
    src/DataBlobWidget.cpp
    src/DataViewer.cpp
)

set(viewer_moc_headers
    DataBlobWidget.h
    DataViewer.h
)

QT4_WRAP_CPP(viewer_src ${viewer_moc_headers})

SUBPACKAGE_LIBRARY( viewer ${viewer_src} )

SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
)

add_subdirectory(test)

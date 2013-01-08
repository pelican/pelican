#
# pelican/viewer/CMakeLists.txt
#

set(module pelican_viewer)
set(${module}_src
    src/DataBlobWidgetFactory.cpp
    src/DataBlobWidget.cpp
    src/DataViewer.cpp
)
set(${module}_moc
    DataBlobWidget.h
    DataViewer.h
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc}
    DEPS pelican_output pelican_comms pelican_utility
    LIBS ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY}
)

add_subdirectory(test)

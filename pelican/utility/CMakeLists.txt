#
# pelican/utility/CMakeLists.txt
#

set(module pelican_utility)

# Thread safe library.
set(${module}_src
    src/ConfigNode.cpp
    src/Config.cpp
    src/ClientTestServer.cpp
    src/PelicanTimeRecorder.cpp
    src/WatchedFile.cpp
    src/WatchedDir.cpp
)
set(${module}_moc_headers
    WatchedFile.h
    WatchedDir.h
    ClientTestServer.h
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc_headers}
    LIBS
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY} 
)

# Recurse into test sub-directory to build tests.
add_subdirectory(test)

#
# pelican/utility/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(utility)

# Thread safe library.
set(utility_src
    src/ConfigNode.cpp
    src/Config.cpp
    src/ClientTestServer.cpp
    src/PelicanTimeRecorder.cpp
    src/WatchedFile.cpp
    src/WatchedDir.cpp
)
set(utility_moc_headers
    WatchedFile.h
    WatchedDir.h
    ClientTestServer.h
)

QT4_WRAP_CPP(utility_src ${utility_moc_headers})
SUBPACKAGE_LIBRARY(utility ${utility_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Recurse into test sub-directory to build tests.
add_subdirectory(test)

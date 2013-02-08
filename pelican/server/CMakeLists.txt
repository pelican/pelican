#
# pelican/server/CMakeLists.txt
#

set(module pelican_server)

# files requiring MOC pre-processing (i.e. QObjects)
set(${module}_moc
    AbstractDataBuffer.h
    AbstractLockable.h
    AbstractLockableData.h
    LockableServiceData.h
    DataReceiver.h
    ServiceDataBuffer.h
    StreamDataBuffer.h
    PelicanPortServer.h
    PelicanServer.h
    Session.h
    LockableStreamData.h
)
set(${module}_src
    src/AbstractChunker.cpp
    src/AbstractDataBuffer.cpp
    src/AbstractLockable.cpp
    src/ChunkerManager.cpp
    src/LockableServiceData.cpp
    src/DataReceiver.cpp
    src/LockedData.cpp
    src/DataManager.cpp
    src/PelicanServer.cpp
    src/PelicanPortServer.cpp
    src/Session.cpp
    src/LockableStreamData.cpp
    src/StreamDataBuffer.cpp
    src/ServiceDataBuffer.cpp
    src/WritableData.cpp
    src/FileChunker.cpp
    src/DirectoryWatchChunker.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc}
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY}
    DEPS pelican_comms pelican_utility
)


# Decsend into test directory.
add_subdirectory(test)

# Create the server executable.
set(serverExe_src src/main.cpp)
add_executable(pelicanServer ${serverExe_src})
target_link_libraries(pelicanServer ${${module}_LIBRARY})



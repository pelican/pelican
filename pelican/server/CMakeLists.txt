include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
)

SUBPACKAGE(server adapters comms utility)

# files requiring MOC pre-processing (i.e. QObjects)
set(server_moc_headers
    AbstractLockable.h
    AbstractLockableData.h
    LockableData.h
    DataBuffer.h
    DataReceiver.h
    ServiceDataBuffer.h
    StreamDataBuffer.h
    PelicanPortServer.h
    PelicanServer.h
    Session.h
    LockableStreamData.h
)

set(server_src
    src/AbstractChunker.cpp
    src/AbstractLockable.cpp
    src/ChunkerManager.cpp
    src/LockableData.cpp
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
)

# MOC Compile headers.
QT4_WRAP_CPP(server_moc_sources ${server_moc_headers})

# Build the subpackage library.
SUBPACKAGE_LIBRARY(server ${server_src} ${server_moc_sources})

# Specify dependencies.
SUBPACKAGE_ADD_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Recurse into test directory.
add_subdirectory(test)



# Create the server executable.
#===============================================================================
set(serverExe_src
    src/main.cpp
)
add_executable(pelicanServer ${serverExe_src})
target_link_libraries(pelicanServer ${SUBPACKAGE_LIBRARIES} memoryTracerDummy )



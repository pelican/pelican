INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(server comms utility)
# files requiring MOC pre-processing (i.e. QObjects)
set( server_moc_headers
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

set( server_src
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
set( serverExe_src
    src/main.cpp
)

QT4_WRAP_CPP(
    server_moc_sources
    ${server_moc_headers} )

SUBPACKAGE_LIBRARY(server ${server_src} ${server_moc_sources})
SUBPACKAGE_ADD_LIBRARIES(${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY})

add_executable(pelicanServer ${serverExe_src} )
target_link_libraries(pelicanServer ${SUBPACKAGE_LIBRARIES} memoryTracerDummy )

add_subdirectory(test)

INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(server comms utility)
# files requireing mpp preprocessing (i.e. QObjects)
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
     src/LofarChunker.cpp
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

add_library(server ${server_src} ${server_moc_sources})
add_library(server_static STATIC ${server_src} ${server_moc_sources})
SET_TARGET_PROPERTIES(server_static PROPERTIES OUTPUT_NAME "server")
SET_TARGET_PROPERTIES(server_static PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(server PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(server_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)


SUBPACKAGE_ADD_LIBRARIES(server ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY})
target_link_libraries(server ${SUBPACKAGE_LIBRARIES} )
install(TARGETS server DESTINATION ${LIBRARY_INSTALL_DIR} )
add_executable(pelicanServer ${serverExe_src} )
target_link_libraries(pelicanServer server ${SUBPACKAGE_LIBRARIES} memoryTracerDummy )

add_subdirectory(test)

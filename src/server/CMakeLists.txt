INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(server comms utility)
# files requireing mpp preprocessing (i.e. QObjects)
set( server_moc_headers
     Data.h
     DataBuffer.h
     DataReceiver.h
     ServiceDataBuffer.h
     StreamDataBuffer.h
     PelicanPortServer.h
     PelicanServer.h
     Session.h
     StreamData.h
   )
set( server_src
     src/AbstractChunker.cpp
     src/Data.cpp
     src/DataReceiver.cpp
     src/LockedData.cpp
     src/DataManager.cpp
     src/DataBuffer.cpp
     src/PelicanServer.cpp
     src/PelicanPortServer.cpp
     src/Session.cpp
     src/StreamData.cpp
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
SUBPACKAGE_ADD_LIBRARIES(server ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY})
target_link_libraries(server ${SUBPACKAGE_LIBRARIES} )
install(TARGETS server DESTINATION ${LIBRARY_INSTALL_DIR} )
add_executable(pelicanServer ${serverExe_src} )
target_link_libraries(pelicanServer server ${SUBPACKAGE_LIBRARIES} memoryTracerDummy )

add_subdirectory(test)

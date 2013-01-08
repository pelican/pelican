#
# pelican/output/CMakeLists.txt
#


set(module pelican_output)
set(${module}_moc
    DataBlobChunkerClient.h
    DataBlobClient.h
    DataBlobRelay.h
    ThreadedDataBlobClient.h
    ThreadedBlobServer.h
    TCPConnectionManager.h
    AbstractDataBlobClient.h
    ThreadedClientImpl.h
)
set(${module}_src
    src/AbstractOutputStream.cpp
    src/AbstractDataBlobClient.cpp
    src/DataBlobChunker.cpp
    src/DataBlobChunkerClient.cpp
    src/DataBlobClient.cpp
    src/DataBlobFileType.cpp
    src/DataBlobFile.cpp
    src/DataBlobRelay.cpp
    src/DataBlobFileReader.cpp
    src/ThreadedDataBlobClient.cpp
    src/ThreadedClientImpl.cpp
    src/OutputStreamManager.cpp
    src/PelicanTCPBlobServer.cpp
    src/Stream.cpp
    src/TCPConnectionManager.cpp
    src/ThreadedBlobServer.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc}
    DEPS pelican_comms pelican_data pelican_server
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY}
)

set(name pelicanrelay)
add_executable(${name} src/pelicanRelay.cpp)
target_link_libraries(${name} ${${module}_LIBRARY} ${Boost_LIBRARIES})
install(TARGETS ${name} DESTINATION ${BINARY_INSTALL_DIR})

add_subdirectory(test)

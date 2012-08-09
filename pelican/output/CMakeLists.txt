#
# pelican/output/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(output comms data server)

set(output_moc_headers
    DataBlobChunkerClient.h
    DataBlobClient.h
    DataBlobRelay.h
    ThreadedDataBlobClient.h
    ThreadedBlobServer.h
    TCPConnectionManager.h
    AbstractDataBlobClient.h
    ThreadedClientImpl.h
)

set(output_src
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

QT4_WRAP_CPP(output_src ${output_moc_headers})

SUBPACKAGE_LIBRARY(output ${output_src})

# Specify external dependencies.
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)
add_executable( pelicanrelay src/pelicanRelay.cpp )
target_link_libraries( pelicanrelay ${SUBPACKAGE_LIBRARIES} ${Boost_LIBRARIES} )
install(TARGETS pelicanrelay DESTINATION ${BINARY_INSTALL_DIR} )

add_subdirectory(test)

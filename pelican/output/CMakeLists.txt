SUBPACKAGE(output comms data server)

set(output_moc_headers
    DataBlobClient.h
    ThreadedDataBlobClient.h
    ThreadedBlobServer.h
    TCPConnectionManager.h
    AbstractDataBlobClient.h
    ThreadedClientImpl.h
)

set(output_src
    src/AbstractOutputStream.cpp
    src/AbstractDataBlobClient.cpp
    src/DataBlobClient.cpp
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

add_subdirectory(test)

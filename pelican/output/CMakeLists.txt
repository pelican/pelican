SUBPACKAGE(output comms data server)

set ( output_moc_headers
      PelicanTCPBlobServer.h
      ThreadedBlobServer.h
      TCPConnectionManager.h
    )

set( output_src
#     src/AbstractBlobServer.cpp
     src/PelicanTCPBlobServer.cpp
     src/TCPConnectionManager.cpp
     src/ThreadedBlobServer.cpp
)

QT4_WRAP_CPP(output_moc_src ${output_moc_headers})

SUBPACKAGE_LIBRARY(output ${output_src} ${output_moc_src} )

# Specify dependencies.
SUBPACKAGE_ADD_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

add_subdirectory(test)

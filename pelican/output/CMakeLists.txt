SUBPACKAGE(output comms data server)

set(output_moc_headers
    PelicanTCPBlobServer.h
    ThreadedBlobServer.h
    TCPConnectionManager.h
)

<<<<<<< HEAD
set(output_src
    src/AbstractBlobServer.cpp
    src/PelicanTCPBlobServer.cpp
    src/TCPConnectionManager.cpp
    src/ThreadedBlobServer.cpp
=======
set( output_src
#     src/AbstractBlobServer.cpp
     src/PelicanTCPBlobServer.cpp
     src/TCPConnectionManager.cpp
     src/ThreadedBlobServer.cpp
>>>>>>> 03ee30cb832b305edbc3b0322d32f0d3486eced6
)

QT4_WRAP_CPP(output_moc_src ${output_moc_headers})

SUBPACKAGE_LIBRARY(output ${output_src} ${output_moc_src} )

# Specify external dependencies.
SUBPACKAGE_ADD_LIBRARIES(output
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

add_subdirectory(test)

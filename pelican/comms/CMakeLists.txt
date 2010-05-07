include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)

SUBPACKAGE(comms data utility)

set(comms_src
    src/DataBlobResponse.cpp
    src/PelicanClientProtocol.cpp
    src/PelicanProtocol.cpp
    src/ServiceDataRequest.cpp
    src/ServiceDataResponse.cpp
    src/StreamData.cpp
    src/StreamDataRequest.cpp
    src/StreamDataResponse.cpp
)
SUBPACKAGE_LIBRARY(comms ${comms_src})

add_subdirectory(test)

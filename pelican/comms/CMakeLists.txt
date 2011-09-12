#
# pelican/comms/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
set(comms_DEPS data utility)
SUBPACKAGE(comms ${comms_DEPS})

set(comms_src
    src/AbstractClientProtocol.cpp
    src/DataBlobResponse.cpp
    src/DataSupportRequest.cpp
    src/DataSupportResponse.cpp
    src/PelicanClientProtocol.cpp
    src/PelicanProtocol.cpp
    src/ServiceDataRequest.cpp
    src/ServiceDataResponse.cpp
    src/StreamData.cpp
    src/StreamDataRequest.cpp
    src/StreamDataResponse.cpp
)

SUBPACKAGE_LIBRARY(comms ${comms_src})

SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Recurse into test directory.
add_subdirectory(test)

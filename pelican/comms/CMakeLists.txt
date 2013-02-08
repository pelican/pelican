#
# pelican/comms/CMakeLists.txt
#

set(module pelican_comms)
set(${module}_src
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
declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_data pelican_utility
    LIBS 
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Recurse into test directory.
add_subdirectory(test)

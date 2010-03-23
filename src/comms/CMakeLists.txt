SUBPACKAGE(comms data utility)
set( comms_src
     src/PelicanClientProtocol.cpp
     src/PelicanProtocol.cpp
     src/ServiceDataRequest.cpp
     src/ServiceDataResponse.cpp
     src/StreamData.cpp
     src/StreamDataRequest.cpp
     src/StreamDataResponse.cpp
)
add_library(comms ${comms_src} )
target_link_libraries(comms ${SUBPACKAGE_LIBRARIES} )
install(TARGETS comms DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(comms)

add_subdirectory(test)

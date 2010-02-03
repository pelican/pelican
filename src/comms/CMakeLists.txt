SUBPACKAGE(comms data utility)
set( comms_src
     src/AbstractClientProtocol.cpp
     src/AbstractProtocol.cpp
     src/PelicanClientProtocol.cpp
     src/PelicanProtocol.cpp
     src/AcknowledgementRequest.cpp
     src/ServerRequest.cpp
     src/ServerResponse.cpp
     src/ServiceDataRequest.cpp
     src/StreamDataRequest.cpp
)
add_library(comms ${comms_src} )
target_link_libraries(comms ${SUBPACKAGE_LIBRARIES} )
install(TARGETS comms DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(comms)

add_subdirectory(test)

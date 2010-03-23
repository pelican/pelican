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
add_library(comms_static STATIC ${comms_src} )

SET_TARGET_PROPERTIES(comms_static PROPERTIES OUTPUT_NAME "comms")
SET_TARGET_PROPERTIES(comms_static PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(comms PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(comms_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(comms ${SUBPACKAGE_LIBRARIES} )
install(TARGETS comms DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(comms)

add_subdirectory(test)

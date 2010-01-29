SUBPACKAGE(comms utility)
set( comms_src
     src/AbstractClientProtocol.cpp
     src/PelicanClientProtocol.cpp
)
add_library(comms ${comms_src} )
target_link_libraries(comms ${SUBPACKAGE_LIBRARIES} )
install(TARGETS comms DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(comms)

add_subdirectory(test)

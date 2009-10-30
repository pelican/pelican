SUBPACKAGE(server)
set( server_src
)
add_library(server ${server_src} )
target_link_libraries(server ${SUBPACKAGE_LIBRARIES} )
install(TARGETS server DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(server)

add_subdirectory(test)

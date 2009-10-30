SUBPACKAGE(adpaters)
set( adpaters_src
)
add_library(adpaters ${adpaters_src} )
target_link_libraries(adpaters ${SUBPACKAGE_LIBRARIES} )
install(TARGETS adpaters DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(adpaters)

add_subdirectory(test)

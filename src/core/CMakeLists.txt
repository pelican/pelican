SUBPACKAGE(core)
set( core_src
)
add_library(core ${core_src} )
target_link_libraries(core ${SUBPACKAGE_LIBRARIES} )
install(TARGETS core DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(core)

add_subdirectory(test)

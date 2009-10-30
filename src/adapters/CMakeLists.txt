SUBPACKAGE(adapters)
set( adapters_src
)
add_library(adapters ${adapters_src} )
target_link_libraries(adapters ${SUBPACKAGE_LIBRARIES} )
install(TARGETS adapters DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(adapters)

add_subdirectory(test)

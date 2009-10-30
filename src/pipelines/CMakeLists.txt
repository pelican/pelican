SUBPACKAGE(pipelines)
set( pipelines_src
)
add_library(pipelines ${pipelines_src} )
target_link_libraries(pipelines ${SUBPACKAGE_LIBRARIES} )
install(TARGETS pipelines DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(pipelines)

add_subdirectory(test)

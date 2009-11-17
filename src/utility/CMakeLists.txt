SUBPACKAGE(utility)
set( utility_src
    src/memoryTracer.cpp
)
add_library(utility ${utility_src} )
target_link_libraries(utility ${SUBPACKAGE_LIBRARIES} )
install(TARGETS utility DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(utility)

add_subdirectory(test)

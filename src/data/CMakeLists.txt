SUBPACKAGE(data)
set( data_src
     src/DataBlob.cpp
)
add_library(data ${data_src} )
target_link_libraries(data ${SUBPACKAGE_LIBRARIES} )
install(TARGETS data DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(data)

add_subdirectory(test)

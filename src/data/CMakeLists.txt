INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(data)
set( data_src
     src/DataBlob.cpp
)
add_library(data ${data_src} )
target_link_libraries(data ${SUBPACKAGE_LIBRARIES} )
install(TARGETS data DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(data ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(data utility)
set( data_src
     src/DataBlob.cpp
     src/TestDataBlob.cpp
     src/DataRequirements.cpp
     src/ServerRequest.cpp
     src/ServiceDataRequest.cpp
     src/StreamDataRequest.cpp
     src/VisibilityData.cpp
)
add_library(data ${data_src} )
target_link_libraries(data ${SUBPACKAGE_LIBRARIES} )
install(TARGETS data DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(data ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

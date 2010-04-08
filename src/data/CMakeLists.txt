INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(data utility)
set( data_src
     src/DataBlob.cpp
     src/DataBlobFactory.cpp
     src/DataRequirements.cpp
     src/ImageData.cpp
     src/AntennaPositions.cpp
     src/VisibilityPositions.cpp
)
SUBPACKAGE_LIBRARY(data ${data_src} )
#add_library(data_static STATIC ${data_src} )
#SET_TARGET_PROPERTIES(data_static PROPERTIES OUTPUT_NAME "data")
#SET_TARGET_PROPERTIES(data_static PROPERTIES PREFIX "lib")
#SET_TARGET_PROPERTIES(data PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#SET_TARGET_PROPERTIES(data_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)


#target_link_libraries(data ${SUBPACKAGE_LIBRARIES} )
#install(TARGETS data DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(${QT_QTCORE_LIBRARY})

add_subdirectory(test)

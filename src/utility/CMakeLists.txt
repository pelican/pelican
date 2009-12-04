INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(utility)
set( utility_src
    src/memoryTracer.cpp
    src/Config.cpp
    src/ServerRequest.cpp
)
add_library(pelicanutility ${utility_src} )
target_link_libraries(pelicanutility ${SUBPACKAGE_LIBRARIES})
install(TARGETS pelicanutility DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(pelicanutility ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

add_subdirectory(test)

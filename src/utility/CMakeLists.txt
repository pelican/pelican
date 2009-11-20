INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(utility)
set( utility_src
    src/memoryTracer.cpp
    src/Config.cpp	
)
add_library(utility ${utility_src} )
target_link_libraries(utility ${SUBPACKAGE_LIBRARIES} )
install(TARGETS utility DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(utility ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

add_subdirectory(test)

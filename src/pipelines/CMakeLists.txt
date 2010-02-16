INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(pipelines utility data modules core)
set( pipelines_src
    src/MyPipeline.cpp
)
add_library(pipelines ${pipelines_src} )
target_link_libraries(pipelines ${SUBPACKAGE_LIBRARIES} )
install(TARGETS pipelines DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(pipelines ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

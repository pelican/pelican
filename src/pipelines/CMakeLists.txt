INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(pipelines utility data modules core)
set( pipelines_src
    src/TestPipelineVisibilityPrinter.cpp
    src/TestPipelineRawImage.cpp
    src/TestPipelineCalibrateImage.cpp
    src/TestPipelineMultipleImages.cpp
)

# Piplines is a temporary package that will be replaced by a
# pipeline applications project and then pipelines will be linked against
# --- libpelican_pipeline.so ?
add_library(pipelines ${pipelines_src} )
target_link_libraries(pipelines ${SUBPACKAGE_LIBRARIES} )
install(TARGETS pipelines DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(pipelines ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

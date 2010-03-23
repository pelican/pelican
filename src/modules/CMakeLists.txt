SUBPACKAGE(modules data utility)
set( modules_src
     src/AbstractModule.cpp
     src/AbstractAstrometry.cpp
     src/EmptyModule.cpp
     src/AstrometryFast.cpp
     src/VisibilityPrinter.cpp
     src/BasicFlagger.cpp
     src/ZenithCalibrater.cpp
     src/ZenithModelVisibilities.cpp
     src/ZenithImagerDft.cpp
     src/ImageWriterFits.cpp
     src/ImageCombiner.cpp
)

add_library(modules ${modules_src} )
add_library(modules_static STATIC ${modules_src})
SET_TARGET_PROPERTIES(modules_static PROPERTIES OUTPUT_NAME "modules")
SET_TARGET_PROPERTIES(modules_static PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(modules PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(modules_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)


target_link_libraries(modules ${SUBPACKAGE_LIBRARIES} )
install(TARGETS modules DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(modules
    ${PELICAN_CBLAS_LIBS}
    ${PELICAN_LAPACK_LIBS}
    ${CFITSIO_LIBRARIES}
)

add_subdirectory(test)

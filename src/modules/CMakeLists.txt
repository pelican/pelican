SUBPACKAGE(modules data utility)
set( modules_src
     src/AbstractAstrometry.cpp
     src/AstrometryFast.cpp
     src/VisibilityPrinter.cpp
     src/BasicFlagger.cpp
     src/EmptyModule.cpp
     src/ZenithCalibrater.cpp
     src/ZenithModelVisibilities.cpp
     src/ZenithImagerDft.cpp
     src/ImageWriterFits.cpp
     src/ImagerDft.cpp
     src/ImagerFft.cpp
)
add_library(modules ${modules_src} )
target_link_libraries(modules ${SUBPACKAGE_LIBRARIES} )
install(TARGETS modules DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(modules
    ${PELICAN_CBLAS_LIBS}
    ${PELICAN_LAPACK_LIBS}
    ${CFITSIO_LIBRARIES}
)

#/usr/lib/libpthread.so

add_subdirectory(test)

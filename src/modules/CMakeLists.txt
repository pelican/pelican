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


include_directories(${MKL_INCLUDE_DIR})
SUBPACKAGE_LIBRARY(modules ${modules_src} )
SUBPACKAGE_ADD_LIBRARIES(
    ${PELICAN_CBLAS_LIBS}
    ${PELICAN_LAPACK_LIBS}
    ${CFITSIO_LIBRARIES}
)

add_subdirectory(test)

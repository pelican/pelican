INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)

SUBPACKAGE(modules data utility)

set(modules_src
    src/AbstractModule.cpp
    src/AbstractAstrometry.cpp
    src/EmptyModule.cpp
    src/AstrometryFast.cpp
    src/VisibilityPrinter.cpp
    src/BasicFlagger.cpp
    #src/ZenithCalibrater.cpp
    src/ZenithModelVisibilities.cpp
    src/ZenithImagerDft.cpp
    src/ImageWriterFits.cpp
    src/ImageCombiner.cpp
)


SUBPACKAGE_LIBRARY(modules ${modules_src})

# Link modules with openmp.
set_target_properties(modules PROPERTIES 
    COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
    LINK_FLAGS "${OpenMP_CXX_FLAGS}"
)

SUBPACKAGE_ADD_LIBRARIES(
    ${pelican_math_libs}
    ${CFITSIO_LIBRARIES}
)

add_subdirectory(test)

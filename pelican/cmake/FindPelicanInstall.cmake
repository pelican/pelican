#
# FindPelicanInstall.cmake
#   Find Pelican and dependencies.
#
# Finds include direcotry, library and associated dependencies.
#
# Defines the following variables:
#   PELICAN_FOUND            = True if PELICAN found
#
#   PELICAN_INCLUDE_DIR      = Top level pelican include directory.
#   PELICAN_LIBRARY          = The PELICAN library
#
#   PELICAN_INCLUDES         = Set of include directories needed by PELICAN.
#   PELICAN_LIBRARIES        = Set of libraries required for linking.
#



# Find the top level pelican include directory.
# ==============================================================================
find_path(PELICAN_INCLUDE_DIR pelican
    PATHS
    /usr/include/
    /usr/local/include
)

# Find the pelican library.
# ==============================================================================
find_library(PELICAN_LIBRARY pelican NAMES pelican)
list(APPEND PELICAN_LIBRARIES ${PELICAN_LIBRARY})

# Load Pelican library build configuration and find dependencies.
# ==============================================================================
include(pelicanBuildConfig.cmake) # Load the build config.
if (NOT pelican_mkl)
    set(IGNORE_MKL true)
endif (NOT pelican_mkl)
include(dependencies.cmake)

list(APPEND PELICAN_INCLUDES
    # boost etc include dir ...
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
)

list(APPEND PELICAN_LIBRARIES
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${PELICAN_CBLAS_LIBS}
    ${PELICAN_LAPACK_LIBS}
    ${CFITSIO_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# handle the QUIETLY and REQUIRED arguments.
# ==============================================================================
include(FindPackageHandleCompat)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pelican
        DEFAULT_MSG PELICAN_LIBRARIES PELICAN_INCLUDE_DIR)


# Not sure what this is for yet...
# ==============================================================================
if (NOT PELICAN_FOUND)
    set(PELICAN_LIBRARIES)
endif (NOT PELICAN_FOUND)

# Hide in the cache.
# ==============================================================================
MARK_AS_ADVANCED(PELICAN_LIBRARIES PELICAN_INCLUDE_DIR)

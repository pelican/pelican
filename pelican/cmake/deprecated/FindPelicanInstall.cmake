#
# FindPelicanInstall.cmake
#   Find Pelican and dependencies.
#
# Finds include direcotry, library and associated dependencies.
#
# Defines the following variables:
#   PELICAN_FOUND             = True if PELICAN found
#
#   PELICAN_INCLUDE_DIR       = Top level pelican include directory.
#   PELICAN_LIBRARY           = The PELICAN framework library
#   PELICAN_TESTUTILS_LIBRARY = The PELICAN test utilities library.
#
#   PELICAN_INCLUDES          = Set of include directories needed by PELICAN.
#   PELICAN_LIBRARIES         = Set of libraries required for linking.
#


# Find the top level pelican include directory.
# ==============================================================================
find_path(PELICAN_INCLUDE_DIR pelican
    PATHS
    ${PELICAN_INSTALL_DIR}/include
    $ENV{PELICAN_INSTALL_DIR}/include
    /usr/include/
    /usr/local/include
)
set(PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR})


# Find the pelican library.
# ==============================================================================
find_library(PELICAN_LIBRARY pelican
    NAMES pelican
    PATHS
    ${PELICAN_INSTALL_DIR}/lib
    $ENV{PELICAN_INSTALL_DIR}/lib
    )
set(PELICAN_LIBRARIES ${PELICAN_LIBRARY})

find_library(PELICAN_TESTUTILS_LIBRARY
    pelican-testutils
    NAMES
    pelican-testutils
    PATHS
    ${PELICAN_INSTALL_DIR}/lib
    $ENV{PELICAN_INSTALL_DIR}/lib
)
list(APPEND PELICAN_LIBRARIES ${PELICAN_TESTUTILS_LIBRARY})


# Load Pelican library build configuration and find dependencies.
# ==============================================================================

include(dependencies)

list(APPEND PELICAN_INCLUDES
    ${Boost_PROJECT_OPTIONS_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)

# Handle the QUIETLY and REQUIRED arguments.
# ==============================================================================
include(FindPackageHandleCompat)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PelicanInstall
        DEFAULT_MSG PELICAN_LIBRARIES PELICAN_INCLUDE_DIR)

# Hide in the cache.
# ==============================================================================
#mark_as_advanced()

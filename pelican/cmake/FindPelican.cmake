# FindPelican.cmake
# =============================================================================
#
# CMake script to find the Pelican libraray and its associated dependencies.
#
# Dependencies for Pelican searched for
# - Boost program options
# - Qt (Core, Xml, Network, Gui)
#
# =============================================================================
# Defines the following variables:
#
#   PELICAN_FOUND             = TRUE if Pelican is found.
#   PELICAN_INCLUDE_DIR       = Top level Pelican include directory.
#   PELICAN_INCLUDES          = All include directories in the Pelican header
#                               tree as well as include directories for
#                               external dependencies.
#   PELICAN_LIBRARY           = The Pelican library.
#   PELICAN_TESTUTILS_LIBRARY = The Pelican test utility library.
#   PELICAN_LIBRARIES         = All Pelican libraries.
#
# ============================================================================
# Environment and CMake variables effecting this script.
#
#   PELICAN_INSTALL_DIR = Top level pelican install directory.
#                         This is the root install location of pelican and
#                         if built from source (on linux) will be usually
#                         be either /usr/local or /usr
#
# ============================================================================
# Last Update: 29th January 2013
# =============================================================================


################################################################################
#
#   INCLUDE-variables
#
################################################################################

# Include default error handling macro.
include(FindPackageHandleStandardArgs)

# Find the top level Pelican include directory.
find_path(PELICAN_INCLUDE_DIR pelican_version.h
    HINTS ${PELICAN_INC_DIR}
    PATHS
    $ENV{PELICAN_INSTALL_DIR}/include/pelican
    /usr/include/pelican
    /usr/local/include/pelican
)
# Also include the parent directory (needed for some projects who have
# includes of the form
#     #include <pelican/../*.h>
# and where the pelican include folder is not in a default search path.
get_filename_component(RESULT ${PELICAN_INCLUDE_DIR}/.. ABSOLUTE)
list(APPEND PELICAN_INCLUDE_DIR ${RESULT})

set(PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR})
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/comms)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/comms/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/core)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/core/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/data)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/data/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/emulator)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/emulator/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/modules)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/output)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/output/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/server)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/server/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/utility)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/utility/test)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/viewer)
list(APPEND PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR}/viewer/test)

################################################################################
#
#   LIBRARY-variables
#
################################################################################

# Find the Pelican library.
find_library(PELICAN_LIBRARY pelican
    NAMES pelican
    HINTS ${PELICAN_LIB_DIR}
    PATHS
    $ENV{PELICAN_INSTALL_DIR}/lib
    /usr/lib
    /usr/local/lib
)
set(PELICAN_LIBRARIES ${PELICAN_LIBRARY})


# Find the Pelican test utility library.
find_library(PELICAN_TESTUTILS_LIBRARY pelican-testutils
    NAMES
    pelican-testutils
    HINTS ${PELICAN_LIB_DIR}
    PATHS
    $ENV{PELICAN_INSTALL_DIR}/lib
    /usr/lib
    /usr/local/lib
)
list(APPEND PELICAN_LIBRARIES ${PELICAN_TESTUTILS_LIBRARY})

################################################################################
#
#  Find depencencies.
#
################################################################################

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml QtGui REQUIRED)
list(APPEND PELICAN_INCLUDES
    ${Boost_PROJECT_OPTIONS_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)

################################################################################
#
#   Checks
#
################################################################################

# Check for errors.
find_package_handle_standard_args(PELICAN
    "ERROR: Could not find Pelican include directory."
    PELICAN_INCLUDE_DIR
)
find_package_handle_standard_args(PELICAN
    "ERROR: Could not find Pelican library."
    PELICAN_LIBRARY
)
find_package_handle_standard_args(PELICAN
    "ERROR: Could not find Pelican test utility library."
    PELICAN_TESTUTILS_LIBRARY
)

# Put variables in the advanced section of the CMake cache.
mark_as_advanced(
    PELICAN_INCLUDE_DIR
    PELICAN_INCLUDES
    PELICAN_LIBRARY
    PELICAN_TESTUTILS_LIBRARY
    PELICAN_LIBRARIES
)


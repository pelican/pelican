# FindPelican.cmake
# =============================================================================
#
# CMake script to find the Pelican libraray and its associated dependencies.
#
# Dependencies for Pelican searched for
# - Boost program options
# - Qt (Core, Xml, Network, Gui)
# - CppUnit
#
# =============================================================================
# Defines the following variables:
#
#   PELICAN_FOUND             = True if Pelican is found.
#
#   PELICAN_CMAKE_MODULE_DIR  = Pelican CMake module path.
#
#   PELICAN_INCLUDE_DIR       = Top level Pelican include directory.
#   PELICAN_INCLUDES          = Set of include directories needed by Pelican.
#
#   PELICAN_LIBRARY           = The Pelican library.
#   PELICAN_TESTUTILS_LIBRARY = The Pelican test utility library.
#   PELICAN_LIBRARIES         = Set of libraries required for linking.
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
# Known Issues
#   - This script is untested with either Mac OS or Windows.
#
# ============================================================================
# Last Update: 26 Oct 2010
# =============================================================================


# Include default error handling macro.
include(FindPackageHandleStandardArgs)


# Find the top level Pelican include directory.
find_path(PELICAN_INCLUDE_DIR pelican
    PATHS
    ${PELICAN_INSTALL_DIR}/include
    $ENV{PELICAN_INSTALL_DIR}/include
    /usr/include
    /usr/local/include
)
set(PELICAN_INCLUDES ${PELICAN_INCLUDE_DIR})


# Find the Pelican library.
find_library(PELICAN_LIBRARY pelican
    NAMES pelican
    PATHS
    ${PELICAN_INSTALL_DIR}/lib
    $ENV{PELICAN_INSTALL_DIR}/lib
    /usr/lib
    /usr/local/lib
)
set(PELICAN_LIBRARIES ${PELICAN_LIBRARY})


# Find the Pelican test utility library.
find_library(PELICAN_TESTUTILS_LIBRARY pelican-testutils
    NAMES
    pelican-testutils
    PATHS
    ${PELICAN_INSTALL_DIR}/lib
    $ENV{PELICAN_INSTALL_DIR}/lib
    /usr/lib
    /usr/local/lib
)
list(APPEND PELICAN_LIBRARIES ${PELICAN_TESTUTILS_LIBRARY})


# Find Pelican cmake modules.
find_path(PELICAN_CMAKE_MODULE_DIR FindPelicanInstall.cmake
    PATHS
    ${PELICAN_INSTALL_DIR}/share/pelican/cmake
    $ENV{PELICAN_INSTALL_DIR}/share/pelican/cmake
    /usr/
    /usr/share
    /usr/share/pelican
    /usr/share/pelican/cmake
    /usr/local/
    /usr/local/share
    /usr/local/share/pelican
    /usr/local/share/pelican/cmake
    PATH_SUFFIXES
    share
    cmake
    pelican
    DOC
    "Location of Pelican cmake modules."
)


# Check for errors.
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pelican
    "ERROR: Could not find Pelican include directory."
    PELICAN_INCLUDE_DIR
)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pelican
    "ERROR: Could not find Pelican library."
    PELICAN_LIBRARY
)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pelican
    "ERROR: Could not find Pelican test utility library."
    PELICAN_TESTUTILS_LIBRARY
)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pelican
    "ERROR: Could not find Required Pelican cmake module path"
    PELICAN_CMAKE_MODULE_DIR
)


# Add the Pelican CMake modules into the CMake module path.
list(INSERT CMAKE_MODULE_PATH 0 "${PELICAN_CMAKE_MODULE_DIR}")
list(INSERT CMAKE_INCLUDE_PATH 0 "${PELICAN_CMAKE_MODULE_DIR}")



# Find dependencies.
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtNetwork QtXml REQUIRED)
find_package(CppUnit REQUIRED)
list(APPEND PELICAN_INCLUDES
    ${Boost_PROJECT_OPTIONS_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)
include_directories(${PELICAN_INCLUDES})


# Put variables in the advanced section of the CMake cache.
mark_as_advanced(
    PELICAN_FOUND
    PELICAN_CMAKE_MODULE_DIR
    PELICAN_INCLUDE_DIR
    PELICAN_INCLUDES
    PELICAN_LIBRARY
    PELICAN_TESTUTILS_LIBRARY
    PELICAN_LIBRARIES
)


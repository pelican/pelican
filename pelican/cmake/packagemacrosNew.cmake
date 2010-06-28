# Define and create the sub-package work directory.
set(SUBPACKAGE_WORK_DIR ${CMAKE_BINARY_DIR}/_subpackages)
file(MAKE_DIRECTORY ${SUBPACKAGE_WORK_DIR})

# Define macros for creating sub-packages.
include(cmake/subpackage.cmake)

# Defined macros for creating project libraries (from sub-packages).
include(cmake/projectLibrary.cmake)


# Known problems / issues :
# ------------------------------------------------------------------------------
#
# = Creation of both static and shared project files for the same library
#   probably not needed as both of these have redundancy in variables.
#
# = linking of subpackage libraries to get ldd correct for shared mode.
#
# = Linking of the minimal set of external libraries for binaries.
#
# = A number of macros from the original package macros are currently missing.
#
# = Documentation.


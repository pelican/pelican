# Define and create the sub-package work directory.
set(SUBPACKAGE_WORK_DIR ${CMAKE_BINARY_DIR}/_subpackages)
file(MAKE_DIRECTORY ${SUBPACKAGE_WORK_DIR})

# Define macros for creating sub-packages.
include(cmake/subpackage.cmake)

# Defined macros for creating project libraries (from sub-packages).
include(cmake/projectLibrary.cmake)



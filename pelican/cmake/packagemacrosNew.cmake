# Define and create the sub-package work directory.
set(SUBPACKAGE_WORK_DIR ${CMAKE_BINARY_DIR}/_subpackages)
file(MAKE_DIRECTORY ${SUBPACKAGE_WORK_DIR})

include(cmake/subpackage.cmake)
include(cmake/projectLibrary.cmake)
# - Find a LAPACK library (no includes)
# This module defines
#  LAPACK_LIBRARIES, the libraries needed to use LAPACK.
#  LAPACK_FOUND, If false, do not try to use LAPACK.
# also defined, but not for general use are
#  LAPACK_LIBRARY, where to find the LAPACK library.


SET(LAPACK_NAMES ${LAPACK_NAMES} lapack)
FIND_LIBRARY(LAPACK_LIBRARY
  NAMES ${LAPACK_NAMES}
  PATHS /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib
)

IF (LAPACK_LIBRARY)
  SET(LAPACK_LIBRARIES ${LAPACK_LIBRARY})
ENDIF (LAPACK_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set LAPACK_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LAPACK DEFAULT_MSG
    LAPACK_LIBRARY LAPACK_LIBRARIES
)

# Deprecated declarations.
GET_FILENAME_COMPONENT (NATIVE_LAPACK_LIB_PATH ${LAPACK_LIBRARY} PATH)

MARK_AS_ADVANCED(LAPACK_LIBRARY)

# - Find the MKL libraries (no includes)
# This module defines
#  MKL_LIBRARIES, the libraries needed to use Intel's implementation of BLAS & LAPACK.
#  MKL_FOUND, If false, do not try to use MKL.

# Architecture specfic interface layer
# ==============================================================================
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET(MKL_NAMES ${MKL_NAMES} mkl_intel_lp64)
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
  SET(MKL_NAMES ${MKL_NAMES} mkl_intel)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

# Computation layer:  see http://bit.ly/bMCczV
# ==============================================================================
SET(MKL_NAMES ${MKL_NAMES} mkl_core)
#SET(MKL_NAMES ${MKL_NAMES} mkl_lapack)

#SET(MKL_NAMES ${MKL_NAMES} mkl) # Library dispatcher for dynamic load of processor specific kernel

# Threading model
# ==============================================================================
#SET(MKL_THREADED true)

IF (MKL_THREADED)
    #SET(MKL_NAMES ${MKL_NAMES} mkl_pgi_thread)

    IF (CMAKE_COMPILER_IS_GNUCXX)
        SET(MKL_NAMES ${MKL_NAMES} mkl_gnu_thread)
    ELSE (CMAKE_COMPILER_IS_GNUCXX)
        SET(MKL_NAMES ${MKL_NAMES} mkl_intel_thread)
    ENDIF (CMAKE_COMPILER_IS_GNUCXX)

    FIND_PACKAGE( OpenMP REQUIRED)
    LIST(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    LIST(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})

    # ?? might need some of this later ??
    #SET(MKL_NAMES ${MKL_NAMES} iomp5) # should link against this for gnu g++ probably rather than the built in openMP lib :\
    # SET(MKL_NAMES ${MKL_NAMES} pthread) # If dont want to use openMP for threading?!
    # Threading library for dynamic linking (needed if use the compilers own omp or pthreads?)
    #SET(MKL_NAMES ${MKL_NAMES} guide)

ELSE (MKL_THREADED)
    SET(MKL_NAMES ${MKL_NAMES} mkl_sequential)
ENDIF (MKL_THREADED)

FOREACH (MKL_NAME ${MKL_NAMES})
  FIND_LIBRARY(${MKL_NAME}_LIBRARY
    NAMES ${MKL_NAME}
    PATHS /usr/lib64 /usr/lib /usr/local/lib64 /usr/local/lib /opt/intel/mkl/*/lib/em64t /opt/intel/mkl/*/lib/32
    )

  SET(TMP_LIBRARY ${${MKL_NAME}_LIBRARY})

  IF(TMP_LIBRARY)
    SET(MKL_LIBRARIES ${MKL_LIBRARIES} ${TMP_LIBRARY})
  ENDIF(TMP_LIBRARY)
ENDFOREACH(MKL_NAME)

IF (MKL_LIBRARIES)
  SET(MKL_FOUND "YES")
ELSE (MKL_LIBRARIES)
  SET(MKL_FOUND "NO")
ENDIF (MKL_LIBRARIES)

IF (MKL_FOUND)
  IF (NOT MKL_FIND_QUIETLY)
    MESSAGE(STATUS "Found MKL libraries: ${MKL_LIBRARIES}")
  ENDIF (NOT MKL_FIND_QUIETLY)
ELSE (MKL_FOUND)
  IF (MKL_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find MKL libraries")
  ENDIF (MKL_FIND_REQUIRED)
ENDIF (MKL_FOUND)

# MARK_AS_ADVANCED(MKL_LIBRARY)

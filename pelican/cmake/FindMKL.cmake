# Find Intel MKL.
################################################################################
# This module defines
#  MKL_FOUND:     If false, do not try to use MKL.
#  MKL_LIBRARIES: The libraries needed to use MKL BLAS & LAPACK.
#  MKL_INCLUDE_DIR:
################################################################################


# Architecture specfic interface layer
# ==============================================================================
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_NAMES ${MKL_NAMES} mkl_intel_lp64)
else (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_NAMES ${MKL_NAMES} mkl_intel)
endif (CMAKE_SIZEOF_VOID_P EQUAL 8)


# Computation layer:  see http://bit.ly/bMCczV
# ==============================================================================
set(MKL_NAMES ${MKL_NAMES} mkl_core)
#SET(MKL_NAMES ${MKL_NAMES} mkl_lapack) # only needed for extended LAPACK functions.
# Library dispatcher for dynamic load of processor specific kernel
#SET(MKL_NAMES ${MKL_NAMES} mkl)


# Threading model
# ==============================================================================
#SET(MKL_THREADED true)

if (MKL_THREADED)

    if (CMAKE_COMPILER_IS_GNUCXX)
        set(MKL_NAMES ${MKL_NAMES} mkl_gnu_thread)
    else (CMAKE_COMPILER_IS_GNUCXX)
        set(MKL_NAMES ${MKL_NAMES} mkl_intel_thread)
    endif (CMAKE_COMPILER_IS_GNUCXX)
    # For pgi compiler use set(MKL_NAMES ${MKL_NAMES} mkl_pgi_thread)

    find_package(OpenMP REQUIRED)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    list(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})

else (MKL_THREADED)

    set(MKL_NAMES ${MKL_NAMES} mkl_sequential)

endif (MKL_THREADED)

# Loop over required library names adding to MKL_LIBRARIES.
foreach (MKL_NAME ${MKL_NAMES})

    find_library(${MKL_NAME}_LIBRARY
        NAMES ${MKL_NAME}
        PATHS
        /usr/lib64
        /usr/lib
        /usr/local/lib64
        /usr/local/lib
        /opt/intel/mkl/*/lib/*/
        /opt/intel/Compiler/*/*/mkl/lib/*/
    )

    set(TMP_LIBRARY ${${MKL_NAME}_LIBRARY})

    if (TMP_LIBRARY)
        set(MKL_LIBRARIES ${MKL_LIBRARIES} ${TMP_LIBRARY})
    endif(TMP_LIBRARY)

endforeach  (MKL_NAME)


find_path(MKL_INCLUDE_DIR mkl.h
    PATHS
    /usr/include/
    /opt/intel/mkl/*/include/*/
    /opt/intel/Compiler/*/*/mkl/include/
)

# Handle the QUIETLY and REQUIRED arguments and set MKL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleCompat)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MKL DEFAULT_MSG MKL_LIBRARIES)

# Hide the MKL_LIBRARY temporary variable in the cmake cache.
mark_as_advanced(MKL_LIBRARY)

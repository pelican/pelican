#===============================================================================
# Find Intel MKL.
#===============================================================================
# This module defines
#  MKL_FOUND:     If false, do not try to use MKL.
#  MKL_LIBRARIES: The libraries needed to use MKL BLAS & LAPACK.
#  MKL_INCLUDE_DIR: The location of mkl.h
#===============================================================================


# Architecture specfic interface layer.
# ==============================================================================
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_NAMES ${MKL_NAMES} mkl_intel_lp64)
else (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(MKL_NAMES ${MKL_NAMES} mkl_intel)
endif (CMAKE_SIZEOF_VOID_P EQUAL 8)


# Computation layer:  see http://bit.ly/bMCczV.
# ==============================================================================
set(mkl_lib_names
    mkl_core
    #mkl_lapack # only needed for extended LAPACK functions.
)


# Threading model.
# ==============================================================================
SET(use_threaded_mkl false)

if (use_threaded_mkl)
    if (CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND ${mkl_lib_names} mkl_gnu_thread)
    else (CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND ${mkl_lib_names} mkl_intel_thread)
    endif (CMAKE_COMPILER_IS_GNUCXX)
    find_package(OpenMP REQUIRED)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    list(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})
else (use_threaded_mkl)
    list(APPEND ${mkl_lib_names} mkl_sequential)
endif (use_threaded_mkl)


# Loop over required library names adding to MKL_LIBRARIES.
# ==============================================================================
foreach (mkl_name ${mkl_lib_names})
    find_library(${mkl_name}_LIBRARY
        NAMES ${mkl_name}
        PATHS
        /usr/lib64
        /usr/lib
        /usr/local/lib64
        /usr/local/lib
        /opt/intel/mkl/*/lib/*/
        /opt/intel/Compiler/*/*/mkl/lib/*/
    )

    set(tmp_library ${${mkl_name}_LIBRARY})

    if (tmp_library)
        set(MKL_LIBRARIES ${MKL_LIBRARIES} ${tmp_library})
    endif (tmp_library)
endforeach (mkl_name)


# Find the include directory.
# ==============================================================================
find_path(MKL_INCLUDE_DIR mkl.h
    PATHS
    /usr/include/
    /opt/intel/mkl/*/include/*/
    /opt/intel/Compiler/*/*/mkl/include/
)

# Handle the QUIETLY and REQUIRED arguments.
# ==============================================================================
include(FindPackageHandleCompat)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MKL DEFAULT_MSG MKL_LIBRARIES)


# Hide in cache.
# ==============================================================================
mark_as_advanced(MKL_LIBRARIES MKL_INCLUDE_DIR tmp_library)

#
# Find Intel MKL.
#
# WARNING:
#   THIS SCRIPT WILL MOST LIKELY FAIL TRUE 64BIT PROCESSORS SUCH AS
#   Intel Itanium 2.
#
# Defines:
#  MKL_FOUND:
#  MKL_LIBRARIES:   The libraries needed to use MKL BLAS & LAPACK.
#  MKL_INCLUDE_DIR
#

# Find the include directory.
# ==============================================================================
find_path(MKL_INCLUDE_DIR mkl.h
    PATHS
    /opt/intel/Compiler/*/*/mkl/include/
    /opt/intel/mkl/*/include/
    /usr/include/
)

# Set the architecture specfic interface layer library.
# ==============================================================================
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(mkl_lib_names mkl_intel_lp64)
    set(intel_64 true)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(mkl_lib_names mkl_intel)
    set(intel_64 false)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

# Set the computation layer library:  see http://bit.ly/bMCczV.
# ==============================================================================
list(APPEND mkl_lib_names
    mkl_core
    # mkl_lapack # only needed for extended LAPACK functions.
)

# Set the threading model.
# ==============================================================================
set(use_threaded_mkl false)

if(use_threaded_mkl)
    if(CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND mkl_lib_names mkl_gnu_thread)
    else(CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND mkl_lib_names mkl_intel_thread)
    endif(CMAKE_COMPILER_IS_GNUCXX)
    find_package(OpenMP REQUIRED)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    list(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})
else(use_threaded_mkl)
    list(APPEND mkl_lib_names mkl_sequential)
endif(use_threaded_mkl)


# Loop over required library names adding to MKL_LIBRARIES.
# ==============================================================================
foreach(mkl_lib ${mkl_lib_names})
    if (intel_64)
        find_library(${mkl_lib}_LIBRARY
            NAMES ${mkl_lib}
            PATHS
            /opt/intel/Compiler/*/*/mkl/lib/em64t/
            /opt/intel/mkl/*/lib/em64t/
            /usr/lib64
            /usr/local/lib64)
    else (intel_64)
        find_library(${mkl_lib}_LIBRARY
            NAMES ${mkl_lib}
            PATHS
            /opt/intel/Compiler/*/*/mkl/lib/32/
            /opt/intel/mkl/*/lib/32/
            /usr/lib
            /usr/local/lib)
    endif (intel_64)
    set(tmp_library ${${mkl_lib}_LIBRARY})
    if (tmp_library)
        list(APPEND MKL_LIBRARIES ${tmp_library})
    endif(tmp_library)
endforeach(mkl_lib ${mkl_lib_names})

# Handle the QUIETLY and REQUIRED arguments.
# ==============================================================================
include(FindPackageHandleCompat)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MKL DEFAULT_MSG MKL_LIBRARIES)

#if(NOT MKL_FOUND)
#    set(MKL_LIBRARIES)
#endif(NOT MKL_FOUND)

# Hide in cache.
# ==============================================================================
mark_as_advanced(MKL_LIBRARIES tmp_library)

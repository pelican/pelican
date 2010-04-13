# This modules finds and sets CBLAS and LAPACK for use with Pelican.
# If MKL is found it will be used in place of CBLAS from either ATLAS or GSL
#-----------------------------------------------------------------------------
# This module defines:
# - PELICAN_CBLAS_LIBS, the libraries needed to use cblas
# - PELICAN_LAPACK_LIBS, the libraries needed to use lapack
# This module sets the compiler definitions
#  - USE_BLAS: if blas is found
#  - USE_MKL:  if mkl blas is found

set(PELICAN_USE_LAPACK false)
set(PELICAN_USE_BLAS false)

if (NOT IGNORE_MLK)
    find_package(MKL)
endif (NOT IGNORE_MLK)

if (MKL_FOUND)

    add_definitions(-DUSE_MKL)
    set(PELICAN_USE_BLAS true)
    set(PELICAN_USE_LAPACK true)
    set(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${MKL_LIBRARIES})
    set(PELICAN_LAPACK_LIBS ${PELICAN_LAPACK_LIBS} ${MKL_LIBRARIES})

else (MKL_FOUND)

    find_package(CBLAS REQUIRED)
    find_package(LAPACK REQUIRED)

    if (LAPACK_FOUND)
        set(PELICAN_USE_LAPACK true)
        set(PELICAN_CBLAS_LIBS ${PELICAN_LAPACK_LIBS} ${LAPACK_LIBRARIES})
    endif (LAPACK_FOUND)

    if (CBLAS_FOUND)
        set(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${CBLAS_LIBRARIES})
        set(PELICAN_USE_BLAS true)
    endif(CBLAS_FOUND)

endif (MKL_FOUND)

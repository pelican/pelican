# This modules finds and sets CBLAS and LAPACK for use with Pelican.
# If MKL is found it will be used in place of CBLAS from either ATLAS or GSL
#-----------------------------------------------------------------------------
# This module defines:
# - PELICAN_CBLAS_LIBS, the libraries needed to use cblas
# - PELICAN_LAPACK_LIBS, the libraries needed to use lapack
# This module sets the compiler definitions
#  - USE_BLAS: if blas is found
#  - USE_MKL:  if mkl blas is found

SET(PELICAN_USE_LAPACK false)
SET(PELICAN_USE_BLAS   false)

IF (NOT IGNORE_MLK)
    FIND_PACKAGE(MKL)
ENDIF (NOT IGNORE_MLK)

IF (MKL_FOUND)

    ADD_DEFINITIONS(-DUSE_MKL)
    SET(PELICAN_USE_BLAS true)
    SET(PELICAN_USE_LAPACK true)
    SET(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${MKL_LIBRARIES})
    SET(PELICAN_LAPACK_LIBS ${PELICAN_LAPACK_LIBS} ${MKL_LIBRARIES})

ELSE (MKL_FOUND)

    FIND_PACKAGE(CBLAS REQUIRED)
    FIND_PACKAGE(LAPACK REQUIRED)

    IF (LAPACK_FOUND)
        SET(PELICAN_USE_LAPACK true)
        SET(PELICAN_CBLAS_LIBS ${PELICAN_LAPACK_LIBS} ${LAPACK_LIBRARIES})
    ENDIF (LAPACK_FOUND)

    IF(CBLAS_FOUND)
        SET(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${CBLAS_LIBRARIES})
        SET(PELICAN_USE_BLAS true)
    ENDIF(CBLAS_FOUND)

ENDIF (MKL_FOUND)



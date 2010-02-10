# CMake Build File
project(Pelican CXX C)

# -- cmake setup info
cmake_minimum_required(VERSION 2.6)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/packagemacros.cmake)
include_directories(${PROJECT_SOURCE_DIR})

# options and defaults
if (NOT CMAKE_BUILD_TYPE ) 
    set( CMAKE_BUILD_TYPE DEBUG )
endif(NOT CMAKE_BUILD_TYPE )

if (TIMER)
    ADD_DEFINITIONS(-DTIMER_ENABLE=1)
endif(TIMER)

if (OPENMP)
    FIND_PACKAGE( OpenMP REQUIRED)
    set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS})
    set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS})
endif (OPENMP)

set( BUILD_SHARED_LIBS TRUE )

if(NOT LIBRARY_INSTALL_DIR )
    set( LIBRARY_INSTALL_DIR lib )
endif(NOT LIBRARY_INSTALL_DIR )

if(NOT PELICAN_INSTALL_DIR )
    set( PELICAN_INSTALL_DIR "${LIBRARY_INSTALL_DIR}/pelican" )
endif(NOT PELICAN_INSTALL_DIR )

if(NOT INCLUDE_INSTALL_DIR )
    set( INCLUDE_INSTALL_DIR "include/pelican" )
endif(NOT INCLUDE_INSTALL_DIR )

if(NOT ICON_INSTALL_DIR )
    set( ICON_INSTALL_DIR "${PELICAN_INSTALL_DIR}/icons" )
endif(NOT ICON_INSTALL_DIR )


SET(PELICAN_USE_LAPACK false)
SET(PELICAN_USE_BLAS   false)

if (NOMKL)
    INCLUDE( ARMA_FindMKL )
endif (NOMKL)

IF (MKL_FOUND) 
    ADD_DEFINITIONS(-DUSE_MKL)
    ADD_DEFINITIONS(-DUSE_BLAS)
    SET(PELICAN_USE_BLAS true)
    SET(PELICAN_USE_LAPACK true)
    SET(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${MKL_LIBRARIES})

ELSE (MKL_FOUND)

    INCLUDE( ARMA_FindCBLAS )
    INCLUDE( ARMA_FindLAPACK )
    
    IF (LAPACK_FOUND)
        SET(PELICAN_USE_LAPACK true)
        SET(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${CLAPACK_LIBRARIES})
    ENDIF (LAPACK_FOUND)
    
    IF(CBLAS_FOUND)
        ADD_DEFINITIONS(-DUSE_BLAS)
        SET(PELICAN_CBLAS_LIBS ${PELICAN_CBLAS_LIBS} ${CBLAS_LIBRARIES})
        SET(PELICAN_USE_BLAS true)
    ENDIF(CBLAS_FOUND)
    
ENDIF (MKL_FOUND)


# Project version
set( ${PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${PROJECT_NAME}_MINOR_VERSION 0 )
set( ${PROJECT_NAME}_PATCH_LEVEL 0 )

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )

# -- dependencies
set(dependencies
        Qt4
        CppUnit
)
PACKAGE_DEPENDENCIES( ${dependencies} )

# -- compiler defaults
SET(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})
if(CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
endif(CMAKE_COMPILER_IS_GNUCC)

IF(APPLE)
    add_definitions(-DDARWIN)
ENDIF(APPLE)

# -- sub modules
include(modules.cmake)

# -- doxygen documentation targets
include(UseDoxygen)
set(DOXYFILE_LATEX "NO")
add_doxygen_target(code DoxyfileCode)
add_doxygen_target(user DoxyfileUser)
add_doxygen_target(developer DoxyfileDeveloper)


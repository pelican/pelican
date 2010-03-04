# CMake Build File
project(Pelican CXX C)

# -- cmake setup info
cmake_minimum_required(VERSION 2.6)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/packagemacros.cmake)
include_directories(${PROJECT_SOURCE_DIR})
enable_testing()

# --- options and defaults
if(NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE DEBUG)
endif(NOT CMAKE_BUILD_TYPE )

if(TIMER)
    ADD_DEFINITIONS(-DTIMER_ENABLE)
endif(TIMER)

if(NO_MKL)
    set( IGNORE_MLK true)
endif(NO_MKL)

if (OPENMP)
    FIND_PACKAGE(OpenMP REQUIRED)
    add_definitions(-DPELICAN_OPENMP)
    LIST(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    LIST(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})
endif (OPENMP)

set( BUILD_SHARED_LIBS TRUE )

if(NOT LIBRARY_INSTALL_DIR )
    set( LIBRARY_INSTALL_DIR lib )
endif (NOT LIBRARY_INSTALL_DIR )

if(NOT PELICAN_INSTALL_DIR )
    set( PELICAN_INSTALL_DIR "${LIBRARY_INSTALL_DIR}/pelican" )
endif (NOT PELICAN_INSTALL_DIR )

if(NOT INCLUDE_INSTALL_DIR )
    set( INCLUDE_INSTALL_DIR "include/pelican" )
endif(NOT INCLUDE_INSTALL_DIR )

if(NOT ICON_INSTALL_DIR )
    set( ICON_INSTALL_DIR "${PELICAN_INSTALL_DIR}/icons" )
endif(NOT ICON_INSTALL_DIR )

# Project version
set( ${PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${PROJECT_NAME}_MINOR_VERSION 0 )
set( ${PROJECT_NAME}_PATCH_LEVEL 0 )

FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )

if (NOT NO_CBLAS)
    include(MathLibraries)
endif (NOT NO_CBLAS)

# -- dependencies
set(dependencies
        Qt4
        CppUnit
        CFitsio
)
PACKAGE_DEPENDENCIES( ${dependencies} )

# -- compiler defaults
SET(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})
IF (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -pedantic -std=c++0x)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
ELSE (CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
    add_definitions(-Wall)
    add_definitions(-wd383 -wd981)  # suppress remark/warning
    add_definitions(-ww111 -ww1572) # promote remark to warning
ENDIF (CMAKE_COMPILER_IS_GNUCC)


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


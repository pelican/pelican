#===============================================================================
# Top level PELICAN cmake script.
#===============================================================================
# Options:
#   -DCMAKE_BUILD_TYPE={debug | release} (default: debug):
#       Build in debug or release mode.
#
#   -DINTEL={true | false} (default: false):
#       Use the intel C++ compiler.
#
#   -DTIMER={true | false} (default: false):
#       Enable additional timer printing.
#
#===============================================================================

# === Specify minimum cmake version.
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Set the compiler.
if (INTEL)
    set(CMAKE_CXX_COMPILER icpc)
    set(CMAKE_C_COMPILER icc)
endif (INTEL)

# === CMake Build File.
project(Pelican CXX C)

# === Project version.
set( ${PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${PROJECT_NAME}_MINOR_VERSION 0 )
set( ${PROJECT_NAME}_PATCH_LEVEL 0 )

# === CMake setup info.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/packagemacros.cmake)
enable_testing()

# === Options and defaults.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif (NOT CMAKE_BUILD_TYPE)

if (TIMER)
    add_definitions(-DTIMER_ENABLE)
endif (TIMER)

# === installtion locations.
if (NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR lib)
endif (NOT LIBRARY_INSTALL_DIR)

if (NOT PELICAN_INSTALL_DIR)
    set(PELICAN_INSTALL_DIR "${LIBRARY_INSTALL_DIR}/pelican")
endif (NOT PELICAN_INSTALL_DIR )

if (NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/pelican")
endif (NOT INCLUDE_INSTALL_DIR)

# === Installation target for project cmake files.
install(DIRECTORY cmake DESTINATION ${PELICAN_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.cmake" )

# === Compiler defaults.
set (BUILD_SHARED_LIBS TRUE)
find_package(OpenMP REQUIRED)
list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
list(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})
set(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})
include_directories(${PROJECT_SOURCE_DIR}/..)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -fPIC")
if (CMAKE_COMPILER_IS_GNUCC)
    # Note that -std=c++0x is needed for -pedantic.
    add_definitions(-Wall -Wextra -pedantic -std=c++0x)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
else (CMAKE_COMPILER_IS_GNUCC) #assume compiler is intel ?!
    add_definitions(-Wall -Wcheck)
    add_definitions(-wd383 -wd981)  # suppress remark/warning
    add_definitions(-ww111 -ww1572) # promote remark to warning
endif (CMAKE_COMPILER_IS_GNUCC)

if (APPLE)
    add_definitions(-DDARWIN)
endif (APPLE)

# === Setup project dependencies.
include(cmake/dependencies.cmake)

# === Sub modules (recurse into sub-directories to build modules).
include(modules.cmake)

# === Doxygen documentation targets.
include(UseDoxygen)
set(DOXYFILE_LATEX "NO")
add_doxygen_target(code DoxyfileCode)
add_doxygen_target(user DoxyfileUser)
add_doxygen_target(developer DoxyfileDeveloper)

# === libpelican.so build target.
PROJECT_LIBRARY(pelican)

# === Write build configuration for use with FindPelican.cmake
file(WRITE cmake/pelicanBuildConfig.cmake "set(pelican_mkl ${pelican_mkl})")

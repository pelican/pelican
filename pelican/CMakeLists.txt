#===============================================================================
# Top level PELICAN cmake script.
#===============================================================================
# Useful CMake Options:
# ---------------------
#   -DCMAKE_BUILD_TYPE={debug | release} (default: debug):
#       Build in debug or release mode.
#
#   -DCMAKE_CXX_COMPILER={compiler} (default: c++)
#       Sets the C++ compiler.
#
#   -DCMAKE_C_COMPILER={compiler} (default: gcc)
#       Sets the C compiler.
#
# Pelican Options:
# ----------------
#   -DTIMER={true | false} (default: false):
#       Enable additional timer printing.
#
#   -DBUILD_STATIC={off|on} (defual: Off):
#       Build static versions of the libraries.
#
# Options manipulating install location:
# --------------------------------------
#   -DCMAKE_INSTALL_PREFIX
#       Root directory used to install files when calling 'make install'
#
#   -DLIBRARY_INSTALL_DIR
#       Pelican library install location. Appending to the install prefix.
#
#   -DINCLUDE_INSTALL_DIR
#       Pelican include headers install location. Appending to the install prefix.
#
#   -DMODULES_INSTALL_DIR
#       Pelican cmake modules install location. Appending to the install prefix.
#===============================================================================

# === Specify minimum cmake version.
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Set the project name (creates name_BINARY_DIR and name_SOURCE_DIR).
project(pelican CXX C)

# === Project version.
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 0)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

# === CMake setup.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/packagemacros.cmake)
enable_testing()

# === Options.
option(TIMER "Enabled pelican timing macros" OFF)
if(TIMER)
    add_definitions(-DTIMER_ENABLE)
endif(TIMER)
option(BUILD_STATIC "Build static versions of pelican library" OFF)
#option(BUILD_SINGLE_LIB "Build a single pelican library" OFF)


# === Installtion locations.
if(NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR lib)
endif(NOT LIBRARY_INSTALL_DIR)

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/pelican")
endif(NOT INCLUDE_INSTALL_DIR)

if(NOT MODULES_INSTALL_DIR)
    set (MODULES_INSTALL_DIR "share/pelican")
endif(NOT MODULES_INSTALL_DIR)

# === Installation target for project cmake files.
install(DIRECTORY cmake DESTINATION ${MODULES_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.cmake")

# === Compiler defaults.
include(cmake/compilerDefaults.cmake)
add_definitions(-DUSE_MEMORY_TRACKER) # needed for release mode too?

# === Setup project dependencies.
include(cmake/dependencies.cmake)

# === Save the build configuration for use with FindPelicanInstall.cmake.
include(cmake/saveBuildConfig.cmake)

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
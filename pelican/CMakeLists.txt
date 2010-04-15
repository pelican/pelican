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
# Options manipulating install location:
# --------------------------------------
#   -DLIBRARY_INSTALL_DIR
#   -DINCLUDE_INSTALL_DIR
#   -DPELICAN_INSTALL_DIR
#===============================================================================

# === Specify minimum cmake version.
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Set the project name (creates name_BINARY_DIR and name_SOURCE_DIR).
project(Pelican CXX C)

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
if (TIMER)
    add_definitions(-DTIMER_ENABLE)
endif (TIMER)

# === Installtion locations.
if (NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR lib)
endif (NOT LIBRARY_INSTALL_DIR)

if (NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/pelican")
endif (NOT INCLUDE_INSTALL_DIR)

if (NOT MODULES_INSTALL_DIR)
    set (MODULES_INSTALL_DIR "share/pelican")
endif (NOT MODULES_INSTALL_DIR)

#if (NOT PELICAN_INSTALL_DIR)
#    set(PELICAN_INSTALL_DIR "${LIBRARY_INSTALL_DIR}/pelican")
#endif (NOT PELICAN_INSTALL_DIR )

# === Installation target for project cmake files.
install(DIRECTORY cmake DESTINATION ${MODULES_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.cmake")

message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "LIBRARY_INSTALL_DIR = ${LIBRARY_INSTALL_DIR}")
message(STATUS "INCLUDE_INSTALL_DIR = ${INCLUDE_INSTALL_DIR}")
message(STATUS "MODULES_INSTALL_DIR = ${MODULES_INSTALL_DIR}")


# === Compiler defaults.
include(cmake/compilerDefaults.cmake)

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

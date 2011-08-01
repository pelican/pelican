#===============================================================================
# Top level ${PROJECT_NAME} cmake script.
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
# Build Options:
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
#       library install location. Appending to the install prefix.
#
#   -DINCLUDE_INSTALL_DIR
#       include headers install location. Appending to the install prefix.
#
#   -DMODULES_INSTALL_DIR
#       cmake modules install location. Appending to the install prefix.
#===============================================================================

# === Specify minimum cmake version.
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Set the project name (creates name_BINARY_DIR and name_SOURCE_DIR).
project(${project_name} CXX C)

# === Project version.
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 0)
set(${PROJECT_NAME}_PATCH_LEVEL 0)

# === CMake setup.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/packagemacrosNew.cmake)
include(cmake/tags.cmake)

enable_testing()

# === Options.
option(TIMER "Enabled ${project_name} timing macros" OFF)
if(TIMER)
    add_definitions(-DTIMER_ENABLE)
endif(TIMER)
option(BUILD_STATIC "Build static versions of ${project_name} library" OFF)
#option(BUILD_SINGLE_LIB "Build a single ${project_name} library" OFF)


# === Installtion locations.
if(NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR "lib")
endif(NOT LIBRARY_INSTALL_DIR)

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/${project_name}")
endif(NOT INCLUDE_INSTALL_DIR)

if(NOT MODULES_INSTALL_DIR)
    set (MODULES_INSTALL_DIR "share/${project_name}")
endif(NOT MODULES_INSTALL_DIR)

# === Installation target for project cmake files.
install(DIRECTORY cmake DESTINATION ${MODULES_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.cmake")

# === Compiler defaults.
include(compilerDefaults)
add_definitions(-DUSE_MEMORY_TRACKER) # needed for release mode too?

# === Setup project dependencies.
include(dependencies)
#_${PROJECT_NAME}_PIC()

#message(STATUS "BUILD_STATIC = ${BUILD_STATIC}")
list(INSERT CMAKE_MODULE_PATH 0 "/usr/local/bang")
#include(bang)
#BANG_VERSION(0.1)


# === Set Broken Package flags.
if(QT_VERSION_MINOR LESS 3)
    add_definitions(-DBROKEN_QT_SET_HEADER)
endif(QT_VERSION_MINOR LESS 3)

# Declare project libraries to be created from the following subpackages.
DECLARE_PROJECT_LIBRARY(${project_name}
    comms core data emulator modules output server utility viewer)

DECLARE_PROJECT_LIBRARY(${project_name}-testutils
    utilityTest viewerTest outputTest dataTest coreTest serverTest emulatorTest)

# === Sub modules (recurse into sub-directories to build modules).
include(modules.cmake)

# Project libraries to produce.
CREATE_PROJECT_LIBRARY(${project_name})
CREATE_PROJECT_LIBRARY(${project_name}-testutils)
if(BUILD_SINGLE_LIB)
    target_link_libraries(${project_name}-testutils
        ${project_name})
endif(BUILD_SINGLE_LIB)

# === Doxygen documentation targets.
include(UseDoxygen)
set(PDFLATEX_COMPILER TRUE)
add_doxygen_target(api DoxyfileAPI)
add_doxygen_target(developer DoxyfileDeveloper)
add_doxygen_target(user DoxyfileUser)


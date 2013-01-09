#===============================================================================
# Top level PELICAN CMake script.
#===============================================================================
# Build options:
#
#   -DCMAKE_BUILD_TYPE={Debug|Release|RelWithDebInfo} (default: Debug):
#       Build in debug, release or release with debug info mode.
#
#   -DCMAKE_CXX_COMPILER={compiler} (default: c++)
#       Sets the C++ compiler.
#
#   -DCMAKE_INSTALL_PREFIX
#       Root directory used to install files when calling 'make install'
#       The default for this is usually /usr/local.
#
#   -DBUILD_STATIC={ON|OFF} (defualt: OFF):
#       Build static libraries.
#
#   -DBUILD_SHARED={ON|OFF} (defualt: ON):
#       Build shared libraries.
#
#   -DBUILD_MODLES={ON|OFF} (defualt: OFF):
#       Build module libraries.
#
#   -DBUILD_PACAKGES={ON|OFF} (defual: ON):
#       Build package libraries.
#
#===============================================================================

# === Set the project name and specify the minumum cmake version.
project(pelican CXX C)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# === Project version.
set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_PATCH 0)
set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# === CMake setup.
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(tags) # Add a target to create a tags file for emacs/vim (untested!)

# === Options for controlling the modular build system.
include(build_macros)
set_build_option_defaults()
set(BUILD_PACKAGES ON)
set(BUILD_MODULES OFF)
set(BUILD_SHARED ON)
set(BUILD_STATIC OFF)

# === Enable the CMake testing framework.
enable_testing()

# === Other options.
option(TIMER "Enabled pelican timing macros" OFF)
if(TIMER)
    add_definitions(-DTIMER_ENABLE)
endif(TIMER)

# === Installtion locations (note these will be prefixed by ${CMAKE_INSTALL_PREFIX})
if(NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR "lib")
endif(NOT LIBRARY_INSTALL_DIR)

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/pelican")
endif(NOT INCLUDE_INSTALL_DIR)

if(NOT MODULES_INSTALL_DIR)
    set (MODULES_INSTALL_DIR "share/pelican")
endif(NOT MODULES_INSTALL_DIR)

if(NOT BINARY_INSTALL_DIR)
    set(BINARY_INSTALL_DIR "bin/")
endif(NOT BINARY_INSTALL_DIR)

# === Set project dependencies and compiler settings.
include(dependencies)
include(compiler_settings)

# === Set Broken Package flags.
if(QT_VERSION_MINOR LESS 3)
    add_definitions(-DBROKEN_QT_SET_HEADER)
endif(QT_VERSION_MINOR LESS 3)

# === Declare project (package) libraries to be created from the listed modules.
# Modules that make up the main pelican library.
declare_package_library(pelican
    pelican_utility
    pelican_emulator
    pelican_data
    pelican_comms
    pelican_server
    pelican_output
    pelican_core
    pelican_viewer
)
# Modules that make up the test utility library.
declare_package_library(pelican-testutils
    pelican_utilityTestUtils
    pelican_emulatorTestUtils
    pelican_dataTestUtils
    pelican_serverTestUtils
    pelican_outputTestUtils
    pelican_coreTestUtils
    pelican_viewerTestUtils
)

# === Sub modules (recurse into sub-directories to build modules).
include(modules.cmake)

#print_module_summary()

# === Create targets to build and install packages and/or modules.
create_library_targets(${PROJECT_VERSION} ${LIBRARY_INSTALL_DIR})

# === Create install target for headers.
create_header_install_target(${INCLUDE_INSTALL_DIR})

# === Create a header file with the project version.
configure_file(${PROJECT_SOURCE_DIR}/cmake/pelican_version.h.in
    ${PROJECT_BINARY_DIR}/pelican_version.h)
install(FILES ${PROJECT_BINARY_DIR}/pelican_version.h 
    DESTINATION ${INCLUDE_INSTALL_DIR})

# === Installation target for project cmake files.
install(DIRECTORY cmake 
    DESTINATION ${MODULES_INSTALL_DIR}
    FILES_MATCHING PATTERN "*.cmake")

# === Include packaging script (still needs some work!)
include(packaging)

# === Doxygen documentation targets.
include(UseDoxygen)
set(PDFLATEX_COMPILER TRUE)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doc")
add_doxygen_target(api  DoxyfileAPI)
add_doxygen_target(dev  DoxyfileDeveloper)
add_doxygen_target(user DoxyfileUser)

# === Print build options summary.
set(DEBUG_PRINT ON) 
if (DEBUG_PRINT)
message(STATUS "")
print_build_options()
message(STATUS "")
message(STATUS "CMakeflags (${CMAKE_PROJECT_NAME} v${PROJECT_VERSION}):")
message(STATUS "  CMAKE_BUILD_TYPE:  ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "C++:")
message(STATUS "  C++ Compiler:         ${CMAKE_CXX_COMPILER}")
message(STATUS "  C++ (Release):        ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "  C++ (RelWithDebInfo): ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "  C++ (Debug):          ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "")
message(STATUS "Documentation targets:")
message(STATUS "  'make user_doc' - Builds library user documentation.")
message(STATUS "  'make api_doc'  - Builds library API documentation.")
message(STATUS "  'make dev_doc'  - Builds framework developer documentation.")
message(STATUS "")
message(STATUS "Install locations: (make install)")
message(STATUS "  Libraries: ${CMAKE_INSTALL_PREFIX}/lib")
message(STATUS "  Inclues:   ${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}")
message(STATUS "  Binaries:  ${CMAKE_INSTALL_PREFIX}/${BINARY_INSTALL_DIR}")
message(STATUS "  Other:     ${CMAKE_INSTALL_PREFIX}/${MODULES_INSTALL_DIR}")
message(STATUS "")
endif (DEBUG_PRINT)

# === Set the compiler
if (INTEL)
  set(CMAKE_CXX_COMPILER icpc)
  set(CMAKE_C_COMPILER icc)
else (INTEL) 
  set(CMAKE_CXX_COMPILER g++)
  set(CMAKE_C_COMPILER gcc)
endif (INTEL)

# === CMake Build File
project(Pelican CXX C)

# === Project version
set( ${PROJECT_NAME}_MAJOR_VERSION 0 )
set( ${PROJECT_NAME}_MINOR_VERSION 0 )
set( ${PROJECT_NAME}_PATCH_LEVEL 0 )

# === CMake setup info
cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(cmake/packagemacros.cmake)
include_directories(${PROJECT_SOURCE_DIR})
enable_testing()

# === Options and defaults
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif (NOT CMAKE_BUILD_TYPE)

if (TIMER)
    add_definitions(-DTIMER_ENABLE)
endif (TIMER)

if (IGNORE_MKL)
    set(IGNORE_MLK true)
endif (IGNORE_MKL)

if (OPENMP)
    find_package(OpenMP REQUIRED)
    add_definitions(-DPELICAN_OPENMP)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
    list(APPEND CMAKE_C_FLAGS ${OpenMP_C_FLAGS})
endif (OPENMP)

if (NOT LIBRARY_INSTALL_DIR)
    set(LIBRARY_INSTALL_DIR lib)
endif (NOT LIBRARY_INSTALL_DIR)

if (NOT PELICAN_INSTALL_DIR)
    set(PELICAN_INSTALL_DIR "${LIBRARY_INSTALL_DIR}/pelican")
endif (NOT PELICAN_INSTALL_DIR )

if (NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include/pelican")
endif (NOT INCLUDE_INSTALL_DIR)

set (BUILD_SHARED_LIBS TRUE)


# === Compiler defaults
set(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})
if (CMAKE_COMPILER_IS_GNUCC)
    # Note that -std=c++0x is needed for -pedantic.
  set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    add_definitions(-Wall -Wextra -pedantic -std=c++0x)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
else (CMAKE_COMPILER_IS_GNUCC) #assume compiler is intel ?!
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    add_definitions(-Wall -Wcheck)
    add_definitions(-wd383 -wd981)  # suppress remark/warning
    add_definitions(-ww111 -ww1572) # promote remark to warning
endif (CMAKE_COMPILER_IS_GNUCC)

if (APPLE)
    add_definitions(-DDARWIN)
endif (APPLE)


# === Dependencies
find_package(Boost COMPONENTS program_options REQUIRED)
include(MathLibraries)
set(dependencies
        Qt4
        CppUnit
        CFitsio
)
PACKAGE_DEPENDENCIES(${dependencies})


# === Sub modules
include(modules.cmake)

# === Doxygen documentation targets
include(UseDoxygen)
set(DOXYFILE_LATEX "NO")
add_doxygen_target(code DoxyfileCode)
add_doxygen_target(user DoxyfileUser)
add_doxygen_target(developer DoxyfileDeveloper)


# === libpelican.so build target
#add_library(pelican SHARED
    #${CMAKE_CURRENT_BUILD_DIR}/adapters/libadapters.a
    #${CMAKE_CURRENT_BUILD_DIR}/comms/libcomms.a
#)
#set_target_properties(pelican PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(pelican ${SUBPACKAGE_LIBRARIES} memoryTracer)
#install(TARGETS pelican DESTINATION ${LIBRARY_INSTALL_DIR})



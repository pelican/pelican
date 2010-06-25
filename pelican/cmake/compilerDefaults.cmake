#
# Compiler defaults for pelican.
# This file is included in the top level pelican CMakeLists.txt.
#

#=== Build in debug mode if not otherwise specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif(NOT CMAKE_BUILD_TYPE)

if(NOT CMAKE_BUILD_TYPE MATCHES "^RELEASE|DEBUG|[Rr]elease|[Dd]ebug$")
    message(FATAL_ERROR "## Unknown build type. Select 'debug' or 'release'")
endif(NOT CMAKE_BUILD_TYPE MATCHES "^RELEASE|DEBUG|[Rr]elease|[Dd]ebug$")

# For now if in release mode build a single library libpelican.so.
# TODO make this an option.
if(CMAKE_BUILD_TYPE MATCHES RELEASE|[Rr]elease)
    set(BUILD_SINGLE_LIB ON)
endif(CMAKE_BUILD_TYPE MATCHES RELEASE|[Rr]elease)

# Option to build shared and static libraries.
set(BUILD_SHARED TRUE)
set(BUILD_STATIC FALSE)

# Check for option consistency.
if(BUILD_SINGLE_LIB)
    if(NOT BUILD_SHARED AND NOT BUILD_STATIC)
        message(FATAL_ERROR "Error: Both STATIC and SHARED single"
            " library targets cannot be disabled in single library mode!")
    endif(NOT BUILD_SHARED AND NOT BUILD_STATIC)
endif(BUILD_SINGLE_LIB)

# Sort out if binaries are to be linked with static or shared libs.
if(BUILD_SHARED AND BUILD_STATIC)
    # Change to TRUE to build static when building both libraries.
    set(STATIC_LINK_BINARIES FALSE)
endif(BUILD_SHARED AND BUILD_STATIC)

if(BUILD_SHARED AND NOT BUILD_STATIC)
    set(STATIC_LINK_BINARIES FALSE)
endif(BUILD_SHARED AND NOT BUILD_STATIC)

if(BUILD_STATIC AND NOT BUILD_SHARED)
    set(STATIC_LINK_BINARIES TRUE)
endif(BUILD_STATIC AND NOT BUILD_SHARED)

# Consistency checking of options
if(STATIC_LINK_BINARIES AND NOT BUILD_STATIC)
    message(FATAL_ERROR "Error: Cannot link binaries with static libraries "
        "with BUILD_STATIC disabled!")
endif(STATIC_LINK_BINARIES AND NOT BUILD_STATIC)

if(NOT STATIC_LINK_BINARIES AND NOT BUILD_SHARED)
    message(FATAL_ERROR "Error: Cannot link binaries with shared libraries "
        "with BUILD_SHARED disabled!")
endif(NOT STATIC_LINK_BINARIES AND NOT BUILD_SHARED)


set(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})

#=== Include the pelican base source directory.
include_directories(${PROJECT_SOURCE_DIR}/..)

# Set the C++ release flags.
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DQT_NO_DEBUG -DNDEBUG")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -Wextra)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
elseif(CMAKE_CXX_COMPILER MATCHES icpc)
    add_definitions(-Wall -Wcheck)
    add_definitions(-wd383 -wd981)  # Suppress remarks / warnings.
    add_definitions(-ww111 -ww1572) # Promote remarks to warnings.
else(CMAKE_COMPILER_IS_GNUCXX)
    # use defaults (and pray it works...)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(APPLE)
    add_definitions(-DDARWIN)
endif(APPLE)





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

set (BUILD_SHARED_LIBS TRUE)

set(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})

#=== Include the pelican base source directory.
include_directories(${PROJECT_SOURCE_DIR}/..)

# Set the C++ release flags.
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DQT_NO_DEBUG -DNDEBUG")

if(CMAKE_COMPILER_IS_GNUCXX)
    # Note that -std=c++0x is needed for -pedantic.
    add_definitions(-Wall -Wextra)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
elseif(CMAKE_CXX_COMPILER MATCHES icpc)
    add_definitions(-Wall -Wcheck)
    add_definitions(-wd383 -wd981)  # suppress remark/warning
    add_definitions(-ww111 -ww1572) # promote remark to warning
    #add_definitions(-static-intel)
    #add_definitions(-pg)
else(CMAKE_COMPILER_IS_GNUCXX)
    # use defaults (and pray it works...)
endif(CMAKE_COMPILER_IS_GNUCXX)

#list(APPEND CMAKE_CXX_FLAGS "-pg")

if(APPLE)
    add_definitions(-DDARWIN)
endif(APPLE)





#
# Compiler defaults for pelican.
# This file is included in the top level pelican CMakeLists.txt.
#

#=== Build in debug mode if not otherwise specified.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif (NOT CMAKE_BUILD_TYPE)

if (NOT CMAKE_BUILD_TYPE MATCHES "^RELEASE|DEBUG|[Rr]elease|[Dd]ebug$")
    message(FATAL_ERROR "## Unknown build type. Select 'debug' or 'release'")
endif (NOT CMAKE_BUILD_TYPE MATCHES "^RELEASE|DEBUG|[Rr]elease|[Dd]ebug$")

set (BUILD_SHARED_LIBS TRUE)

#=== Find and set OpenMP compiler flag.
find_package(OpenMP REQUIRED)
list(APPEND CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
list(APPEND CMAKE_C_FLAGS "${OpenMP_C_FLAGS}")

set(CPP_PLATFORM_LIBS ${CMAKE_THREAD_LIBS_INIT})

#=== Include the pelican base source directory.
include_directories(${PROJECT_SOURCE_DIR}/..)

# Set the C++ release flags.
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DQT_NO_DEBUG -DNDEBUG")

if (CMAKE_COMPILER_IS_GNUCXX)
    # Note that -std=c++0x is needed for -pedantic.
    add_definitions(-Wall -Wextra -pedantic -std=c++0x)
    add_definitions(-Wno-deprecated -Wno-unknown-pragmas)
    list(APPEND CPP_PLATFORM_LIBS util dl)
elseif (CMAKE_CXX_COMPILER MATCHES icpc)
    add_definitions(-Wall -Wcheck)
    add_definitions(-wd383 -wd981)  # suppress remark/warning
    add_definitions(-ww111 -ww1572) # promote remark to warning
else (CMAKE_COMPILER_IS_GNUCXX)
    # use defaults (and pray it works...)
endif (CMAKE_COMPILER_IS_GNUCXX)

if (APPLE)
    add_definitions(-DDARWIN)
endif (APPLE)





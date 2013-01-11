#
# Compiler defaults for pelican.
# This file is included in the top level pelican CMakeLists.txt.
#

# Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

if (CMAKE_CXX_COMPILER MATCHES icpc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcheck -wd2259 -wd1125")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-variadic-macros")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

# Set include directories (these probably are not all needed!)
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/utility
    ${PROJECT_SOURCE_DIR}/utility/test
    ${PROJECT_SOURCE_DIR}/data
    ${PROJECT_SOURCE_DIR}/data/test
    ${PROJECT_SOURCE_DIR}/comms
    ${PROJECT_SOURCE_DIR}/comms/test
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}/server/test
    ${PROJECT_SOURCE_DIR}/output
    ${PROJECT_SOURCE_DIR}/output/test
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/core/test
    ${PROJECT_SOURCE_DIR}/viewer
    ${PROJECT_SOURCE_DIR}/viewer/test
    ${PROJECT_SOURCE_DIR}/examples
    ${QT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)
if (CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
endif()

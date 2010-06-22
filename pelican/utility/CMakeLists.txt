SUBPACKAGE(utility)
# Thread safe library.
set(utility_src
    src/ConfigNode.cpp
    src/Config.cpp
    src/SocketTester.cpp
)
# The general utility library.
SUBPACKAGE_LIBRARY(utility ${utility_src})
SUBPACKAGE_ADD_LIBRARIES(utility
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

#===============================================================================
# Memory tracer.
#===============================================================================

set(memoryTracer_src
    src/memoryTracer.cpp
)

set(memoryTracerDummy_src
    src/memoryTracerDummy.cpp
)

# Build the non-thread safe library.
add_library(memoryTracer ${memoryTracer_src})

# Build the thread safe library (dummies).
add_library(memoryTracerDummy ${memoryTracerDummy_src})

# Only install the memory tracer if debug mode.
if(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)
    install(TARGETS memoryTracerDummy DESTINATION ${LIBRARY_INSTALL_DIR} )
    install(TARGETS memoryTracer DESTINATION ${LIBRARY_INSTALL_DIR} )
endif(CMAKE_BUILD_TYPE MATCHES DEBUG|[Dd]ebug)


# Recurse into test sub-directory to build tests.
add_subdirectory(test)
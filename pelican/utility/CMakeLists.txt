include_directories(${QT_INCLUDES})
SUBPACKAGE(utility)

# Thread safe library.
set(utility_src
    src/ConfigNode.cpp
    src/Config.cpp
    src/SocketTester.cpp
)

# Thread safe dummies.
set(dummyTracer_src
    src/memoryTracerDummy.cpp
)

# Non thread safe library.
set(tracer_src
    src/memoryTracer.cpp
)

# The general utility library.
SUBPACKAGE_LIBRARY(utility ${utility_src})

SUBPACKAGE_ADD_LIBRARIES(
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

# Build the thread safe library (dummies).
add_library(memoryTracerDummy ${dummyTracer_src} )
target_link_libraries(memoryTracerDummy ${SUBPACKAGE_LIBRARIES})

if (DEBUG)
    install(TARGETS memoryTracerDummy DESTINATION ${LIBRARY_INSTALL_DIR} )
endif (DEBUG)

# Build the non-thread safe library.
add_library(memoryTracer ${tracer_src} )
target_link_libraries(memoryTracer ${SUBPACKAGE_LIBRARIES})

if (DEBUG)
    install(TARGETS memoryTracer DESTINATION ${LIBRARY_INSTALL_DIR} )
endif (DEBUG)

# Recurse into test sub-directory to build tests.
add_subdirectory(test)

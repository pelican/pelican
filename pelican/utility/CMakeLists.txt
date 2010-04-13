INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(utility)
# thread safe library
set( utility_src
    src/ConfigNode.cpp
    src/Config.cpp
    src/SocketTester.cpp
    src/AbstractFactory.cpp
)
# thread safe dummies
set( dummyTracer_src
    src/memoryTracerDummy.cpp
)
# non thread safe library
set( tracer_src
    src/memoryTracer.cpp
)

# The general utility library
SUBPACKAGE_LIBRARY(utility ${utility_src} )
SUBPACKAGE_ADD_LIBRARIES(${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

# The thread safe library (dummies)
add_library(memoryTracerDummy ${dummyTracer_src} )
target_link_libraries(memoryTracerDummy ${SUBPACKAGE_LIBRARIES})
IF(DEBUG)
install(TARGETS memoryTracerDummy DESTINATION ${LIBRARY_INSTALL_DIR} )
ENDIF(DEBUG)

# The non-thread safe library
add_library(memoryTracer ${tracer_src} )
target_link_libraries(memoryTracer ${SUBPACKAGE_LIBRARIES})
IF(DEBUG)
install(TARGETS memoryTracer DESTINATION ${LIBRARY_INSTALL_DIR} )
ENDIF(DEBUG)

add_subdirectory(test)

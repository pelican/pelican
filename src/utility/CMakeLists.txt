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
add_library(pelicanutility ${utility_src} )
add_library(pelicanutility_static STATIC ${utility_src} )
SET_TARGET_PROPERTIES(pelicanutility_static PROPERTIES OUTPUT_NAME "pelicanutility")
SET_TARGET_PROPERTIES(pelicanutility_static PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(pelicanutility PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(pelicanutility_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)



target_link_libraries(pelicanutility ${SUBPACKAGE_LIBRARIES})
install(TARGETS pelicanutility DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(pelicanutility ${QT_QTNETWORK_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

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

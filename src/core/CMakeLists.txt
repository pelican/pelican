INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(core modules comms data adapters)
set( core_src
     src/AbstractDataClient.cpp
     src/AbstractPipeline.cpp
     src/DataClientFactory.cpp
     src/DataTypes.cpp
     src/FileDataClient.cpp
     src/ModuleFactory.cpp
     src/PelicanServerClient.cpp
     src/PipelineApplication.cpp
     src/PipelineDriver.cpp
)
add_library(core ${core_src} )
add_library(core_static STATIC ${core_src} )

SET_TARGET_PROPERTIES(core_static PROPERTIES OUTPUT_NAME "core")
SET_TARGET_PROPERTIES(core_static PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(core PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(core_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)


target_link_libraries(core ${SUBPACKAGE_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})
install(TARGETS core DESTINATION ${LIBRARY_INSTALL_DIR} )

SUBPACKAGE_ADD_LIBRARIES(core ${SUBPACKAGE_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

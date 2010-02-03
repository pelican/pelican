INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(core modules comms data)
set( core_src
     src/AbstractDataClient.cpp
     src/AbstractPipeline.cpp
     src/ModuleFactory.cpp
     src/PelicanServerClient.cpp
     src/PipelineApplication.cpp
     src/PipelineDriver.cpp
)
add_library(core ${core_src} )
target_link_libraries(core ${SUBPACKAGE_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY})
install(TARGETS core DESTINATION ${LIBRARY_INSTALL_DIR} )

SUBPACKAGE_ADD_LIBRARIES(core ${SUBPACKAGE_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

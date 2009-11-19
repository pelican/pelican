INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(core data)
set( core_src
     src/AbstractPipeline.cpp
     src/CoreOptions.cpp
     src/DataClient.cpp
     src/DataRequirements.cpp
     src/ModuleFactory.cpp
     src/PipelineApplication.cpp
     src/PipelineDriver.cpp
)
add_library(core ${core_src} )
target_link_libraries(core ${SUBPACKAGE_LIBRARIES} )
install(TARGETS core DESTINATION ${LIBRARY_INSTALL_DIR} )

SUBPACKAGE_ADD_LIBRARIES(core ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

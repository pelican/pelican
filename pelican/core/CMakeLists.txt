include_directories(${QT_INCLUDES})
SUBPACKAGE(core modules comms data adapters)

set( core_src
     src/AbstractDataClient.cpp
     src/DirectStreamDataClient.cpp
     src/AbstractPipeline.cpp
     src/DataClientFactory.cpp
     src/DataTypes.cpp
     src/FileDataClient.cpp
     src/ModuleFactory.cpp
     src/PelicanServerClient.cpp
     src/PipelineApplication.cpp
     src/PipelineDriver.cpp
)
SUBPACKAGE_LIBRARY(core ${core_src} )

SUBPACKAGE_ADD_LIBRARIES(${SUBPACKAGE_LIBRARIES}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
     ${QT_QTCORE_LIBRARY}
)

add_subdirectory(test)

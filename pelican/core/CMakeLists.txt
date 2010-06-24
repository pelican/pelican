SUBPACKAGE(core modules server output comms data)

set(core_src
    src/AbstractDataClient.cpp
    src/AdapterRealData.cpp
    src/DirectStreamDataClient.cpp
    src/AbstractPipeline.cpp
    src/DataClientFactory.cpp
    src/DataTypes.cpp
    src/FileDataClient.cpp
    src/PelicanServerClient.cpp
    src/PipelineApplication.cpp
    src/PipelineDriver.cpp
    src/OutputStreamer.cpp
)

SUBPACKAGE_LIBRARY(core ${core_src})

# Set external library depencencies for core.
SUBPACKAGE_ADD_LIBRARIES(core
    ${QT_QTCORE_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

add_subdirectory(test)

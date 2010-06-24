SUBPACKAGE(core modules server comms data)
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
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

add_subdirectory(test)

#
# pelican/core/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

SUBPACKAGE(core server output comms data)

set(core_src
    src/AbstractDataClient.cpp
    src/AbstractAdaptingDataClient.cpp
    src/AbstractAdapterFactory.cpp
    src/AbstractModule.cpp
    src/AdapterRealData.cpp
    src/DirectStreamDataClient.cpp
    src/AbstractPipeline.cpp
    src/DataClientFactory.cpp
    src/DataTypes.cpp
    src/FileDataClient.cpp
    src/PelicanServerClient.cpp
    src/PipelineApplication.cpp
    src/PipelineDriver.cpp
    src/PipelineSwitcher.cpp
)

SUBPACKAGE_LIBRARY(core ${core_src})

SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

add_subdirectory(test)

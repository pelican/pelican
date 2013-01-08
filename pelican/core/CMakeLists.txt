#
# pelican/core/CMakeLists.txt
#

set(module pelican_core)
set(${module}_src
    src/AbstractDataClient.cpp
    src/AbstractAdaptingDataClient.cpp
    src/AbstractAdapterFactory.cpp
    src/AbstractModule.cpp
    src/AdapterRealData.cpp
    src/DirectStreamDataClient.cpp
    src/AbstractPipeline.cpp
    src/DataClientFactory.cpp
    src/DataBlobAdapter.cpp
    src/DataTypes.cpp
    src/FileDataClient.cpp
    src/PelicanServerClient.cpp
    src/PipelineApplication.cpp
    src/PipelineDriver.cpp
    src/PipelineSwitcher.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_server
         pelican_output
         pelican_comms
         pelican_data
    LIBS ${QT_QTCORE_LIBRARY}
         ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

add_subdirectory(test)

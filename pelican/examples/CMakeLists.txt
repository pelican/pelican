include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# Build example codes (to check they work!)
set(examples_src
    AdapterExample.cpp
    ChunkerExample.cpp
    DirectDataClientExample.cpp
    EmulatorExample.cpp
    ModuleExample.cpp
    OutputStreamExample.cpp
    PipelineExample.cpp
    SignalAmplifier.cpp
    SignalProcessingPipeline.cpp
)
add_library(examples ${examples_src})
target_link_libraries(examples
    ${SUBPACKAGE_LIBRARIES}
)

#
# pelican/examples/CMakeLists.txt
#

set(module pelican_examples)
# Build example codes (to check they work!)
set(${module}_src
    reference/src/AdapterExample.cpp
    reference/src/ChunkerExample.cpp
    reference/src/DirectDataClientExample.cpp
    reference/src/EmulatorExample.cpp
    reference/src/ModuleExample.cpp
    reference/src/PipelineExample.cpp
    tutorial/src/OutputStreamExample.cpp
    tutorial/src/SignalAmplifier.cpp
    tutorial/src/SignalChunker.cpp
    tutorial/src/SignalData.cpp
    tutorial/src/SignalDataAdapter.cpp
    tutorial/src/SignalEmulator.cpp
    tutorial/src/SignalProcessingPipeline.cpp
)

declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_core pelican_server pelican_emulator
)

# Create the pipeline binary.
add_executable(signalPipeline tutorial/src/mainPipeline.cpp)
target_link_libraries(signalPipeline ${${module}_LIBRARY})

# Create the server binary.
add_executable(signalServer tutorial/src/mainServer.cpp)
target_link_libraries(signalServer ${${module}_LIBRARY})

# Create the emulator binary.
add_executable(signalEmulator tutorial/src/mainEmulator.cpp)
target_link_libraries(signalEmulator ${${module}_LIBRARY})

# Copy the XML configuration files needed for the tutorial.
include(CopyFiles)
copy_files(${CMAKE_CURRENT_SOURCE_DIR}/tutorial/data/*.xml . exampleXML)

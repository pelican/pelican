#
# pelican/examples/CMakeLists.txt
#

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SUBPACKAGE(examples core server emulator)

# Build example codes (to check they work!)
set(examples_src
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
add_library(examples ${examples_src})
target_link_libraries(examples
    ${SUBPACKAGE_LIBRARIES}
)

# Create the pipeline binary.
add_executable(signalPipeline
    tutorial/src/mainPipeline.cpp
)
target_link_libraries(signalPipeline
    ${SUBPACKAGE_LIBRARIES}
    examples
)

# Create the server binary.
add_executable(signalServer
    tutorial/src/mainServer.cpp
)
target_link_libraries(signalServer
    ${SUBPACKAGE_LIBRARIES}
    examples
)

# Create the emulator binary.
add_executable(signalEmulator
    tutorial/src/mainEmulator.cpp
)
target_link_libraries(signalEmulator
    ${SUBPACKAGE_LIBRARIES}
    examples
)

# Copy the XML configuration files needed for the tutorial.
include(CopyFiles)
copy_files(${CMAKE_CURRENT_SOURCE_DIR}/tutorial/data/*.xml . exampleXML)

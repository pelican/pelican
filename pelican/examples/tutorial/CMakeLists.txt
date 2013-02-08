#
# pelican/examples/tutorial/CMakeLists.txt
#

# ********** FIXME ******************
#TODO create ${PELICAN_LIBRARIES} variable (to mirror FindPELICAN.cake).
#which resolves either to pelican or a list of modules.
set(PELICAN_LIBRARIES 
    ${pelican_core_LIBRARY}
    ${pelican_server_LIBRARY}
    ${pelican_emulator_LIBRARY})
list(REMOVE_DUPLICATES PELICAN_LIBRARIES)
# ********** FIXME ******************

set(lib_name pelican_tutorial)
set(${lib_name}_src
    src/OutputStreamExample.cpp
    src/SignalAmplifier.cpp
    src/SignalChunker.cpp
    src/SignalData.cpp
    src/SignalDataAdapter.cpp
    src/SignalEmulator.cpp
    src/SignalProcessingPipeline.cpp
)
add_library(${lib_name} STATIC ${${lib_name}_src})
target_link_libraries(${lib_name} ${PELICAN_LIBRARIES})

# Create the pipeline binary.
add_executable(signalPipeline src/mainPipeline.cpp)
target_link_libraries(signalPipeline ${lib_name})

# Create the server binary.
add_executable(signalServer src/mainServer.cpp)
target_link_libraries(signalServer ${lib_name})

# Create the emulator binary.
add_executable(signalEmulator src/mainEmulator.cpp)
target_link_libraries(signalEmulator ${lib_name})

# Copy the XML configuration files needed for the tutorial.
include(copy_files)
copy_files(
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    *.xml
    ${CMAKE_CURRENT_BINARY_DIR}
)
copy_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/readme.txt
    ${CMAKE_CURRENT_BINARY_DIR}/readme.txt
)
copy_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/data/plotScript.gnu
    ${CMAKE_CURRENT_BINARY_DIR}/plotScript.gnu
)

# Create a dependency between the pelican_tutorial library target and the
# file copy target to ensure that the xml and readme files are always updated
# when building the example.
add_dependencies(pelican_tutorial copy_files)

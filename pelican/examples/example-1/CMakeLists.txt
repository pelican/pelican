#
# pelican/examples/example-1/CMakeLists.txt
#

include_directories(
    ${PROJECT_SOURCE_DIR}/.. # for #include <pelican/../..> style includes
    ${PROJECT_BINARY_DIR}    # for pelican_version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/emulator
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline
    ${CMAKE_CURRENT_SOURCE_DIR}/server
)

# ********** FIXME ******************
set(PELICAN_LIBRARY pelican)
set(PELICAN_TESTUTILS_LIBRARY pelican-testutils)
# ********** FIXME ******************

# Build the emulator binary.
set(name emulator)
add_executable(${name} 
    emulator/main.cpp 
    emulator/StreamDataEmulator.cpp
    emulator/ServiceDataEmulator.cpp)
target_link_libraries(${name}
    ${PELICAN_LIBRARY}
    ${PELICAN_TESTUTILS_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY})


# Build the server binary.
set(name server)
add_executable(${name} 
    server/main.cpp 
    server/StreamDataChunker.cpp
    server/ServiceDataChunker.cpp)
target_link_libraries(${name}
    ${PELICAN_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY})

# Build the pipeline binary.
set(name pipeline)
add_executable(${name} 
    pipeline/main.cpp
    pipeline/ServiceDataAdapter.cpp
    pipeline/StreamData.cpp
    pipeline/StreamDataAdapter.cpp
    pipeline/Pipeline.cpp
)
target_link_libraries(${name}
    ${PELICAN_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTNETWORK_LIBRARY})

# Copy the XML configuration files needed for the tutorial.
include(copy_files)
copy_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline/pipeline.xml
    ${CMAKE_CURRENT_BINARY_DIR}/pipelineConfig.xml
)
copy_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/server/server.xml
    ${CMAKE_CURRENT_BINARY_DIR}/serverConfig.xml
)
copy_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/readme.txt
    ${CMAKE_CURRENT_BINARY_DIR}/readme.txt
)

# Create a dependency between the pipeline binary target and the file copy
# target to ensure that the xml and readme files are always updated
# when building the example.
add_dependencies(pipeline copy_files)



# =============================================================================
# Untested example binaries
# =============================================================================

#set(name serviceEmulator)
#add_executable(${name} 
#    emulator/mainServiceEmulator.cpp 
#    emulator/ServiceDataEmulator.cpp)
#target_link_libraries(${name}
#    ${PELICAN_LIBRARY}
#    ${PELICAN_TESTUTILS_LIBRARY}
#    ${QT_QTCORE_LIBRARY}
#    ${QT_QTXML_LIBRARY}
#    ${QT_QTNETWORK_LIBRARY})

#set(name streamEmulator)
#add_executable(${name} 
#    emulator/mainStreamEmulator.cpp 
#    emulator/StreamDataEmulator.cpp)
#target_link_libraries(${name}
#    ${PELICAN_LIBRARY}
#    ${PELICAN_TESTUTILS_LIBRARY}
#    ${QT_QTCORE_LIBRARY}
#    ${QT_QTXML_LIBRARY}
#    ${QT_QTNETWORK_LIBRARY})


# Build the direct client pipeline binary.
#set(name pipeline-dc)
#add_executable(${name} 
#    pipeline-dc/main.cpp
#    pipeline-dc/DirectDataClient.cpp
#    server/StreamDataChunker.cpp
#    server/ServiceDataChunker.cpp
#    pipeline/ServiceDataAdapter.cpp
#    pipeline/StreamData.cpp
#    pipeline/StreamDataAdapter.cpp
#    pipeline/Pipeline.cpp
#)
#target_link_libraries(${name}
#    ${PELICAN_LIBRARY}
#    ${QT_QTCORE_LIBRARY}
#    ${QT_QTXML_LIBRARY}
#    ${QT_QTNETWORK_LIBRARY})


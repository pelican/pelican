#
# pelican/emulator/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(emulator utility)

set(emulator_src
    src/AbstractUdpEmulator.cpp
    src/EmulatorDriver.cpp
)

# Headers requiring MOC pre-processing (i.e. QObjects)
set(emulator_moc_headers
    EmulatorDriver.h
)

# MOC Compile headers.
QT4_WRAP_CPP(emulator_src ${emulator_moc_headers})

# Create the emulator subpackage library.
SUBPACKAGE_LIBRARY(emulator ${emulator_src})

# Specify dependencies.
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Recurse into test subdirectory.
add_subdirectory(test)


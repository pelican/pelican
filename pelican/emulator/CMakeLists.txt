SUBPACKAGE(emulator utility)

set(emulator_src
    src/AbstractUdpEmulator.cpp
    src/RealUdpEmulator.cpp
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
SUBPACKAGE_ADD_LIBRARIES(emulator
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Recurse into test subdirectory. 
add_subdirectory(test)


SUBPACKAGE(testutils utility)
set( testutils_src
    src/AbstractEmulator.cpp
    src/AbstractUdpEmulator.cpp
    src/EmulatorDriver.cpp
)
SUBPACKAGE_LIBRARY(testutils ${testutils_src})

add_subdirectory(test)


include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)

SUBPACKAGE(testutils utility)

# files requiring MOC pre-processing (i.e. QObjects)
set(testutils_moc_headers
    EmulatorDriver.h
    TestChunker.h
)

set(testutils_src
    src/AbstractUdpEmulator.cpp
    src/RealUdpEmulator.cpp
    src/EmulatorDriver.cpp
    src/TestChunker.cpp
    src/TestUdpChunker.cpp
)

# MOC Compile headers.
QT4_WRAP_CPP(testutils_src ${testutils_moc_headers})

SUBPACKAGE_LIBRARY(testutils ${testutils_src})

target_link_libraries(testutils server) # Hack as subpackage stuff fails

# Specify dependencies.
SUBPACKAGE_ADD_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

# Recurse 
add_subdirectory(test)


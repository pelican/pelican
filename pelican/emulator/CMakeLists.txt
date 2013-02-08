#
# pelican/emulator/CMakeLists.txt
#

set(module pelican_emulator)
set(${module}_src
    src/AbstractUdpEmulator.cpp
    src/EmulatorDriver.cpp
)
set(${module}_moc
    EmulatorDriver.h
)
declare_module_library(${module}
    SRC ${${module}_src}
    MOC ${${module}_moc}
    DEPS pelican_utility
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY}
)

# Recurse into test subdirectory.
add_subdirectory(test)


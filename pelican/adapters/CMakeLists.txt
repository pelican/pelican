include_directories(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
)

SUBPACKAGE(adapters utility data)

set(adapters_src
    src/AdapterLofarStationVisibilities.cpp
    src/AdapterAntennaList.cpp
)

SUBPACKAGE_LIBRARY(adapters ${adapters_src})

# Recurse into test directory
add_subdirectory(test)

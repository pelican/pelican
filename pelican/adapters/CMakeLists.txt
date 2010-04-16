INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(adapters utility data)

set( adapters_src
    src/AdapterLofarStationVisibilities.cpp
    src/AdapterAntennaList.cpp
)

SUBPACKAGE_LIBRARY(adapters ${adapters_src})
SUBPACKAGE_ADD_LIBRARIES(${QT_QTCORE_LIBRARY})

add_subdirectory(test)

INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(adapters utility data)

set( adapters_src
    src/AdapterFactory.cpp
    src/AdapterLofarStationVisibilities.cpp
    src/AdapterAntennaList.cpp
)

add_library(adapters ${adapters_src} )
target_link_libraries(adapters ${SUBPACKAGE_LIBRARIES} )
install(TARGETS adapters DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(adapters ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

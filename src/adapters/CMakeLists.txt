INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(adapters utility data)

set( adapters_src
    src/AbstractAdapter.cpp
    src/AbstractServiceAdapter.cpp
    src/AbstractStreamAdapter.cpp
    src/AdapterFactory.cpp
    src/AdapterLofarStationVisibilities.cpp
    src/AdapterAntennaList.cpp
)

add_library(adapters ${adapters_src})
add_library(adapters_static STATIC ${adapters_src})
SET_TARGET_PROPERTIES(adapters_static PROPERTIES OUTPUT_NAME "adapters")
SET_TARGET_PROPERTIES(adapters_static PROPERTIES PREFIX "lib")
SET_TARGET_PROPERTIES(adapters PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(adapters_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)



target_link_libraries(adapters ${SUBPACKAGE_LIBRARIES} )
install(TARGETS adapters DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(adapters ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

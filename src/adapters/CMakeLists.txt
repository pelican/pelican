INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(adapters data utility)

set( adapters_src
    src/AbstractAdapter.cpp
    src/AdapterFactory.cpp
    src/FileAdapterLofarVisibilities.cpp
)

add_library(adapters ${adapters_src} )
target_link_libraries(adapters ${SUBPACKAGE_LIBRARIES} )
install(TARGETS adapters DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(adapters ${QT_QTCORE_LIBRARY})

add_subdirectory(test)

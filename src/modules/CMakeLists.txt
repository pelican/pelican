SUBPACKAGE(modules data utility)
set( modules_src
     src/BasicFlagger.cpp
     src/EmptyModule.cpp
     src/ZenithCalibrater.cpp
     src/ZenithImagerDft.cpp
     src/ZenithModelVisibilities.cpp
)
add_library(modules ${modules_src} )
target_link_libraries(modules ${SUBPACKAGE_LIBRARIES} )
install(TARGETS modules DESTINATION ${LIBRARY_INSTALL_DIR} )
SUBPACKAGE_ADD_LIBRARIES(modules)

add_subdirectory(test)

SUBPACKAGE(data utility)
set(data_src
    src/DataBlob.cpp
    src/DataRequirements.cpp
)
SUBPACKAGE_LIBRARY(data ${data_src} )
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
)
add_subdirectory(test)

SUBPACKAGE(data utility)

set(data_src
    src/DataBlob.cpp
    src/DataRequirements.cpp
    src/ArrayData.cpp
)

SUBPACKAGE_LIBRARY(data ${data_src} )

SUBPACKAGE_ADD_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
)

add_subdirectory(test)

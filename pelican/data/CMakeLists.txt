INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
)

SUBPACKAGE(data utility)

set(data_src
    src/AntennaPositions.cpp
    src/CorrectedVisibilityData.cpp
    src/DataBlob.cpp
    src/DataRequirements.cpp
    src/FlagTable.cpp
    src/ImageData.cpp
    src/ModelVisibilityData.cpp
    src/RealData.cpp
    src/VisibilityData.cpp
    src/VisibilityPositions.cpp
)

SUBPACKAGE_LIBRARY(data ${data_src} )

SUBPACKAGE_ADD_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
)

add_subdirectory(test)

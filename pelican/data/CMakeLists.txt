INCLUDE_DIRECTORIES(${QT_INCLUDES})
SUBPACKAGE(data utility)

set(data_src
    src/DataBlob.cpp
    src/DataBlobFactory.cpp
    src/DataRequirements.cpp
    src/ImageData.cpp
    src/AntennaPositions.cpp
    src/VisibilityPositions.cpp
)
SUBPACKAGE_LIBRARY(data ${data_src} )

SUBPACKAGE_ADD_LIBRARIES(${QT_QTCORE_LIBRARY})

add_subdirectory(test)

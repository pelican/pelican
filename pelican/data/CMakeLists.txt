#
# pelican/data/CMakeLists.txt
#

include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(data utility)
set(data_src
    src/DataBlob.cpp
    src/DataBlobBuffer.cpp
    src/DataBlobVerify.cpp
    src/DataRequirements.cpp
    src/DataBlobFactory.cpp
)
SUBPACKAGE_LIBRARY(data ${data_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(${QT_QTCORE_LIBRARY})
add_subdirectory(test)

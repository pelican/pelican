#
# pelican/data/CMakeLists.txt
#

set(module pelican_data)
set(${module}_src
    src/DataBlob.cpp
    src/DataBlobBuffer.cpp
    src/DataBlobVerify.cpp
    src/DataRequirements.cpp
    src/DataSpec.cpp
    src/DataBlobFactory.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_utility
    LIBS ${QT_QTCORE_LIBRARY}
)

add_subdirectory(test)

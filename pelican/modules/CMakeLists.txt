SUBPACKAGE(modules data utility)

set(modules_src
    src/AbstractModule.cpp
)

SUBPACKAGE_LIBRARY(modules ${modules_src})

SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
)

add_subdirectory(test)

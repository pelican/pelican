INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
)

SUBPACKAGE(modules data utility)

set(modules_src
    src/AbstractModule.cpp
    src/EmptyModule.cpp
)

SUBPACKAGE_LIBRARY(modules ${modules_src})

add_subdirectory(test)

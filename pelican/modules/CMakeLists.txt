SUBPACKAGE(modules data utility)

set(modules_src
    src/AbstractModule.cpp
)

SUBPACKAGE_LIBRARY(modules ${modules_src})

add_subdirectory(test)

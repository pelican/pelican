SUBPACKAGE(modules pelican data utility)

set(modules_src
    src/AbstractModule.cpp
    src/EmptyModule.cpp
)

SUBPACKAGE_LIBRARY(modules pelican ${modules_src})

add_subdirectory(test)

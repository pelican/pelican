include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# Build example codes (to check they work!)
set(examples_src
    DirectDataClientExample.cpp
    AdapterExample.cpp
    ChunkerExample.cpp
    PipelineExample.cpp
    ModuleExample.cpp
)
add_library(examples ${examples_src})
target_link_libraries(examples
    ${SUBPACKAGE_LIBRARIES}
)

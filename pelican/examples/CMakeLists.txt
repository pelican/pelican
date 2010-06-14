set( examples_src
     AdapterExample.cpp
     ChunkerExample.cpp
     #PipelineExample.cpp
     DataBlobExample.cpp
     ModuleExample.cpp
)

add_library(examples ${examples_src} )
target_link_libraries(examples
    ${SUBPACKAGE_LIBRARIES}
)

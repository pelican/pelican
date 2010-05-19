SUBPACKAGE(pipelines data modules core server)

set( examples_src
     AdapterExample.cpp
     ChunkerExample.cpp
     PipelineExample.cpp
)

add_library(examples ${examples_src} )
target_link_libraries(examples ${SUBPACKAGE_LIBRARIES} )

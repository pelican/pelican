include(CopyFiles)
SUBPACKAGE(pipelinesTest pipelines)
set(pipelinesTest_src
    src/pipelinesTest.cpp
    src/TestPipelineVisibilityPrinterTest.cpp
    src/TestPipelineDirtyImageTest.cpp
    src/TestPipelineMultipleImagesTest.cpp
)

set(TestPipelineVisibilityPrinter_src
    src/pipelinesTest.cpp
    src/TestPipelineVisibilityPrinterTest.cpp
)

set(TestPipelineDirtyImage_src
    src/pipelinesTest.cpp
    src/TestPipelineDirtyImageTest.cpp
)

set( TestPipelineMultipleImages_src
    src/pipelinesTest.cpp
    src/TestPipelineMultipleImagesTest.cpp
)

add_executable(pipelinesTest ${pipelinesTest_src} )
target_link_libraries(pipelinesTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )

add_executable(testPipelineVisibilityPrinter ${TestPipelineVisibilityPrinter_src})
target_link_libraries(testPipelineVisibilityPrinter ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

add_executable(testPipelineDirtyImage ${TestPipelineDirtyImage_src})
target_link_libraries(testPipelineDirtyImage ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

add_executable(testPipelineMultipleImages ${TestPipelineMultipleImages_src})
target_link_libraries(testPipelineMultipleImages ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

add_test(pipelinesTest pipelinesTest)

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/* . pipeLineTestData)
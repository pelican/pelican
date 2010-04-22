include(CopyFiles)
SUBPACKAGE(pipelinesTest pipelines)

set(pipelinesTest_src
    src/pipelinesTest.cpp
    src/TestPipelineVisibilityPrinterTest.cpp
    src/TestPipelineServerTest.cpp
    src/TestPipelineRawImageTest.cpp
    #src/TestPipelineCalibrateImageTest.cpp
    src/TestPipelineMultipleImagesTest.cpp
)

set(TestPipelineVisibilityPrinter_src
    src/pipelinesTest.cpp
    src/TestPipelineVisibilityPrinterTest.cpp
)

set(TestPipelineServer_src
    src/pipelinesTest.cpp
    src/TestPipelineServerTest.cpp
)

set(TestPipelineRawImage_src
    src/pipelinesTest.cpp
    src/TestPipelineRawImageTest.cpp
)

set(TestPipelineCalibrateImage_src
    src/pipelinesTest.cpp
    src/TestPipelineCalibrateImageTest.cpp
)

set( TestPipelineMultipleImages_src
    src/pipelinesTest.cpp
    src/TestPipelineMultipleImagesTest.cpp
)

# removed single test as its slow
#add_executable(pipelinesTest ${pipelinesTest_src})
#target_link_libraries(pipelinesTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer )

add_executable(testPipelineVisibilityPrinter ${TestPipelineVisibilityPrinter_src})
target_link_libraries(testPipelineVisibilityPrinter ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

add_executable(testPipelineServer ${TestPipelineServer_src})
target_link_libraries(testPipelineServer ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracerDummy)

add_executable(testPipelineRawImage ${TestPipelineRawImage_src})
target_link_libraries(testPipelineRawImage ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

#add_executable(testPipelineCalibrateImage ${TestPipelineCalibrateImage_src})
#target_link_libraries(testPipelineCalibrateImage ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

add_executable(testPipelineMultipleImages ${TestPipelineMultipleImages_src})
target_link_libraries(testPipelineMultipleImages ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES} memoryTracer)

# removed as its slow and artificial (not really a unit test)
#add_test(pipelinesTest pipelinesTest)

copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/* . pipeLineTestData)

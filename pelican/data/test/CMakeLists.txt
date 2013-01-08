#
# pelican/data/test/CMakeLists.txt
#


# Build the PELICAN data testing library module.
set(module pelican_dataTestUtils)
declare_module_library(${module}
    SRC src/TestDataBlob.cpp
    DEPS pelican_data
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY}
)

# Build unit tests.
if (CPPUNIT_FOUND)
    set(dataTest_src
        src/CppUnitMain.cpp
        src/DataRequirementsTest.cpp
        src/DataSpecTest.cpp
        src/DataBlobBufferTest.cpp
        src/DataBlobVerifyTest.cpp
    )
    add_executable(dataTest ${dataTest_src})
    target_link_libraries(dataTest 
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES})
    add_test(dataTest dataTest)
endif (CPPUNIT_FOUND)



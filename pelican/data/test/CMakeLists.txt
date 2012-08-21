#
# pelican/data/test/CMakeLists.txt
#


# Build Pelican data testing library module.
include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(dataTest data)
set(dataTestLib_src
    src/TestDataBlob.cpp
)
SUBPACKAGE_LIBRARY(dataTestUtils ${dataTestLib_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})


if (CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})
    set(dataTest_src
        src/dataTest.cpp
        src/DataRequirementsTest.cpp
        src/DataBlobBufferTest.cpp
        src/DataBlobVerifyTest.cpp
    )
    add_executable(dataTest ${dataTest_src})
    target_link_libraries(dataTest ${SUBPACKAGE_LIBRARIES} ${CPPUNIT_LIBRARIES})
    add_test(dataTest dataTest)
endif (CPPUNIT_FOUND)



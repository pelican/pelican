#
# pelican/utility/test/CMakeLists.txt
#

# Build the Pelican utility testing library module
include_directories(${QT_INCLUDE_DIR})
SUBPACKAGE(utilityTest utility)
set(utilityTestLib_src
    src/SocketTester.cpp
    src/TestFile.cpp
)
SUBPACKAGE_LIBRARY(utilityTestUtils ${utilityTestLib_src})
SUBPACKAGE_SET_EXTERNAL_LIBRARIES(
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)


if (CPPUNIT_FOUND)
    include_directories(${CPPUNIT_INCLUDE_DIR})

    # Build Pelcain utility tests.
    set(utilityTest_src
        src/TypeCounterTest
        src/utilityTest.cpp
        src/SocketTesterTest.cpp
        src/ConfigTest.cpp
        src/ConfigNodeTest.cpp
        src/ContiguousMemoryTest.cpp
        src/CircularBufferIteratorTest.cpp
        src/LockingCircularBufferTest.cpp
        src/PelicanTimeRecorderTest.cpp
    )
    set(utilityTest_mt_src
        src/utilityTest.cpp
        src/WatchedFileTest.cpp
        src/WatchedDirTest.cpp
    )

    add_executable(utilityTestMT ${utilityTest_mt_src} )
    target_link_libraries(utilityTestMT
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )

    add_executable(utilityTest ${utilityTest_src} )
    target_link_libraries(utilityTest
        ${SUBPACKAGE_LIBRARIES}
        ${CPPUNIT_LIBRARIES}
    )

    add_test(utilityTest utilityTest)
    add_test(utilityTestMT utilityTestMT)

    # Copy test xml files to build directory.
    include(CopyFiles)
    copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/*.xml . testXMLFiles)
endif (CPPUNIT_FOUND)

#
# pelican/utility/test/CMakeLists.txt
#

# Build the Pelican utility testing library module
set(module pelican_utilityTestUtils)
set(${module}_src
    src/SocketTester.cpp
    src/TestFile.cpp
)
declare_module_library(${module}
    SRC ${${module}_src}
    DEPS pelican_utility
    LIBS 
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

if (CPPUNIT_FOUND)
    # Build Pelcain utility tests.
    set(utilityTest_src
        src/CppUnitMain.cpp
        src/TypeCounterTest
        src/SocketTesterTest.cpp
        src/ConfigTest.cpp
        src/ConfigNodeTest.cpp
        src/ContiguousMemoryTest.cpp
        src/CircularBufferIteratorTest.cpp
        src/LockingCircularBufferTest.cpp
        src/PelicanTimeRecorderTest.cpp
    )
    set(utilityTest_mt_src
        src/CppUnitMain.cpp
        src/WatchedFileTest.cpp
        src/WatchedDirTest.cpp
    )

    add_executable(utilityTestMT ${utilityTest_mt_src} )
    target_link_libraries(utilityTestMT
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )

    add_executable(utilityTest ${utilityTest_src} )
    target_link_libraries(utilityTest
        ${${module}_LIBRARY}
        ${CPPUNIT_LIBRARIES}
    )

    add_test(utilityTest utilityTest)
    add_test(utilityTestMT utilityTestMT)

    # Copy test xml files to build directory.
    include(CopyFiles)
    copy_files(${CMAKE_CURRENT_SOURCE_DIR}/data/*.xml . testXMLFiles)
endif (CPPUNIT_FOUND)

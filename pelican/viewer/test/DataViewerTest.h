#ifndef DATAVIEWERTEST_H
#define DATAVIEWERTEST_H

class QApplication;
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataViewerTest.h
 */

namespace pelican {

/**
 * @class DataViewerTest
 * 
 * @brief
 *   Unit test for the DataViewer
 * @details
 * 
 */
class DataViewerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataViewerTest );
        CPPUNIT_TEST( test_client );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_client();

    public:
        /// DataViewerTest constructor.
        DataViewerTest();

        /// DataViewerTest destructor.
        ~DataViewerTest();

    private:
        QApplication* _app;
};

} // namespace pelican

#endif // DATAVIEWERTEST_H 

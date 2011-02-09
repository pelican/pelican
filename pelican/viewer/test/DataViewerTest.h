#ifndef DATAVIEWERTEST_H
#define DATAVIEWERTEST_H

class QApplication;
#include <QString>
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataViewerTest.h
 */

namespace pelican {
    class DataBlobClient;
    class PelicanTCPBlobServer;

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
        CPPUNIT_TEST( test_integrationWithDataClient);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_client();
        void test_integrationWithDataClient();

    public:
        /// DataViewerTest constructor.
        DataViewerTest();

        /// DataViewerTest destructor.
        ~DataViewerTest();

    private:
        DataBlobClient* _client( PelicanTCPBlobServer* server );
        QApplication* _app;
};

} // namespace pelican

#endif // DATAVIEWERTEST_H 

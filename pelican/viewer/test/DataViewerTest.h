#ifndef DATAVIEWERTEST_H
#define DATAVIEWERTEST_H

/**
 * @file DataViewerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include <QtCore/QString>

class QApplication;

namespace pelican {

class DataBlobClient;
class PelicanTCPBlobServer;

/**
 * @ingroup t_viewer
 *
 * @class DataViewerTest
 *
 * @brief
 * Unit test for the DataViewer
 *
 * @details
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
        virtual ~DataViewerTest();

    private:
        DataBlobClient* _client( PelicanTCPBlobServer* server );
};

CPPUNIT_TEST_SUITE_REGISTRATION( DataViewerTest );

} // namespace pelican
#endif // DATAVIEWERTEST_H

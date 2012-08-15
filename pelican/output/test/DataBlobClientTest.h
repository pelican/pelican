#ifndef DATABLOBCLIENTTEST_H
#define DATABLOBCLIENTTEST_H

/**
 * @file DataBlobClientTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include <QtCore/QString>

namespace pelican {
class DataBlobClient;
class PelicanTCPBlobServer;

/**
 * @ingroup t_output
 *
 * @class DataBlobClientTest
 *
 * @brief
 * Unit test for the DataBlobClient.
 */

class DataBlobClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobClientTest );
        CPPUNIT_TEST( test_streamInfo );
        CPPUNIT_TEST( test_subscribe );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_streamInfo();
        void test_subscribe();

    public:
        /// DataBlobClientTest constructor.
        DataBlobClientTest();

        /// DataBlobClientTest destructor.
        ~DataBlobClientTest();

    private:
        DataBlobClient* _client(PelicanTCPBlobServer* server, const QString& xml = QString( ));
        PelicanTCPBlobServer* _server(quint16 port = 0 ); // default => find free port

};

} // namespace pelican
#endif // DATABLOBCLIENTTEST_H

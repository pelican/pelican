#ifndef DATABLOBCLIENTTEST_H
#define DATABLOBCLIENTTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <QString>
#include <QCoreApplication>

/**
 * @file DataBlobClientTest.h
 */

namespace pelican {
class DataBlobClient;
class PelicanTCPBlobServer;

/**
 * @class DataBlobClientTest
 * 
 * @brief
 *   unit test for the DataBlobClient
 */
class DataBlobClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobClientTest );
//        CPPUNIT_TEST( test_streamInfo );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_streamInfo();

    public:
        /// DataBlobClientTest constructor.
        DataBlobClientTest();

        /// DataBlobClientTest destructor.
        ~DataBlobClientTest();

    private:
        DataBlobClient* _client(PelicanTCPBlobServer* server, const QString& xml = QString( ));
        PelicanTCPBlobServer* _server(quint16 port = 0 ); // default => find free port

        QCoreApplication* _app;
};

} // namespace pelican

#endif // DATABLOBCLIENTTEST_H 

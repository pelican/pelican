#ifndef THREADEDDATABLOBCLIENTTEST_H
#define THREADEDDATABLOBCLIENTTEST_H

/**
 * @file ThreadedDataBlobClientTest.h
 */

#include <QtCore/QString>
#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

class ThreadedDataBlobClient;
class PelicanTCPBlobServer;

/**
 * @ingroup t_output
 *
 * @class ThreadedDataBlobClientTest
 *
 * @brief
 * Unit test for the ThreadedDataBlobClient
 * @details
 */

class ThreadedDataBlobClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ThreadedDataBlobClientTest );
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
        /// ThreadedDataBlobClientTest constructor.
        ThreadedDataBlobClientTest();

        /// ThreadedDataBlobClientTest destructor.
        ~ThreadedDataBlobClientTest();

    private:
        ThreadedDataBlobClient* _client(PelicanTCPBlobServer* server,
                const QString& xml);
};

} // namespace pelican
#endif // THREADEDDATABLOBCLIENTTEST_H

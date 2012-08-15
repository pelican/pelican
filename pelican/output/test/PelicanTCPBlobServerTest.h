#ifndef PELICAN_TCP_BLOB_SERVER_TEST_H_
#define PELICAN_TCP_BLOB_SERVER_TEST_H_

/**
 * @file PelicanTCPBlobServerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
#include "pelican/utility/Config.h"

namespace pelican {

/**
 * @ingroup t_output
 *
 * @class PelicanTCPBlobServerTest
 *
 * @brief
 * Unit test for the PelicanTCPBlobServer class
 *
 * @details
 */

class PelicanTCPBlobServerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanTCPBlobServerTest );
        CPPUNIT_TEST( test_portConfig );
        CPPUNIT_TEST( test_connection );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_portConfig();
        void test_connection();

    public:
        PelicanTCPBlobServerTest(  );
        ~PelicanTCPBlobServerTest();

    private:
};

} // namespace pelican
#endif // PELICAN_TCP_BLOB_SERVER_TEST_H_

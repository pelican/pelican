#ifndef PELICANSERVERCLIENTTEST_H
#define PELICANSERVERCLIENTTEST_H

/**
 * @file PelicanServerClientTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class PelicanServerClientTest
 *
 * @brief
 * Unit test for the PelicanServerClient class.
 *
 * @details
 */

class PelicanServerClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanServerClientTest );
        CPPUNIT_TEST( test_response );
        CPPUNIT_TEST( test_getData );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_response();
        void test_getData();

    public:
        PelicanServerClientTest(  );
        ~PelicanServerClientTest();

};

} // namespace pelican
#endif // PELICANSERVERCLIENTTEST_H

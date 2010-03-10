#ifndef PELICANSERVERCLIENTTEST_H
#define PELICANSERVERCLIENTTEST_H

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

/**
 * @file PelicanServerClientTest.h
 */

namespace pelican {

/**
 * @class PelicanServerClientTest
 *  
 * @brief
 *  unit test fo the PelicanServerClient class
 * @details
 * 
 */

class PelicanServerClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanServerClientTest );
        CPPUNIT_TEST( test_response );
//        CPPUNIT_TEST( test_getData );
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

    private:
        QCoreApplication* _coreApp;
};

} // namespace pelican
#endif // PELICANSERVERCLIENTTEST_H 

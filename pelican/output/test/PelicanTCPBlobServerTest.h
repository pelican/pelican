#ifndef LOFARCHUNKERTEST_H
#define LOFARCHUNKERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "pelican/utility/Config.h"
class QCoreApplication;

/**
 * @file PelicanTCPBlobServerTest.h
 */

namespace pelican {

/**
 * @class PelicanTCPBlobServerTest
 *
 * @brief
 *   Unit test for the PelicanTCPBlobServer class
 * @details
 *
 */

class PelicanTCPBlobServerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( PelicanTCPBlobServerTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        PelicanTCPBlobServerTest(  );
        ~PelicanTCPBlobServerTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif 

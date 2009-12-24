#ifndef SERVICEDATABUFFERTEST_H
#define SERVICEDATABUFFERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ServiceDataBufferTest.h
 */

namespace pelican {

/**
 * @class ServiceDataBufferTest
 *  
 * @brief
 *    Unit test for ServiceDataBuffer
 * @details
 * 
 */

class ServiceDataBufferTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ServiceDataBufferTest );
        CPPUNIT_TEST( test_getWritable );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_getWritable();

    public:
        ServiceDataBufferTest(  );
        ~ServiceDataBufferTest();

    private:
};

} // namespace pelican
#endif // SERVICEDATABUFFERTEST_H 

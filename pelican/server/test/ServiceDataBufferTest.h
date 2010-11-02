#ifndef SERVICEDATABUFFERTEST_H
#define SERVICEDATABUFFERTEST_H

/**
 * @file ServiceDataBufferTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class ServiceDataBufferTest
 *
 * @brief
 * Unit test for ServiceDataBuffer
 *
 * @details
 */

class ServiceDataBufferTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ServiceDataBufferTest );
        CPPUNIT_TEST( test_getWritable );
        CPPUNIT_TEST( test_retiredData );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_getWritable();
        void test_retiredData();

    public:
        ServiceDataBufferTest(  );
        ~ServiceDataBufferTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif // SERVICEDATABUFFERTEST_H

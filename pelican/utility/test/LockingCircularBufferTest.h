#ifndef LOCKINGCIRCULARBUFFERTEST_H
#define LOCKINGCIRCULARBUFFERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file LockingCircularBufferTest.h
 */

namespace pelican {

/**
 * @class LockingCircularBufferTest
 *  
 * @brief
 *   unit test for the locking circular buffer test template
 * @details
 * 
 */

class LockingCircularBufferTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( LockingCircularBufferTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        LockingCircularBufferTest(  );
        ~LockingCircularBufferTest();

    private:
};

} // namespace pelican
#endif // LOCKINGCIRCULARBUFFERTEST_H 

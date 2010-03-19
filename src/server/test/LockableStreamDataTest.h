#ifndef LOCKABLESTREAMDATATEST_H
#define LOCKABLESTREAMDATATEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file LockableStreamDataTest.h
 */

namespace pelican {

/**
 * @class LockableStreamDataTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class LockableStreamDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( LockableStreamDataTest );
        CPPUNIT_TEST( test_addAssociates );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_addAssociates();

    public:
        LockableStreamDataTest(  );
        ~LockableStreamDataTest();

    private:
};

} // namespace pelican
#endif // LOCKABLESTREAMDATATEST_H 

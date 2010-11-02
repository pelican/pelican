#ifndef LOCKEDDATATEST_H
#define LOCKEDDATATEST_H

/**
 * @file LockedDataTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class LockedDataTest
 *
 * @brief
 * Unit test for LockedData class
 *
 * @details
 */

class LockedDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( LockedDataTest );
        CPPUNIT_TEST( test_isValid );
//        CPPUNIT_TEST( test_size );
        CPPUNIT_TEST( test_lock );

        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_isValid();
//        void test_size();
        void test_lock();

    public:
        LockedDataTest(  );
        ~LockedDataTest();

    private:
};

} // namespace pelican
#endif // LOCKEDDATATEST_H

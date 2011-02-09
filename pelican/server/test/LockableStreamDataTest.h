#ifndef LOCKABLESTREAMDATATEST_H
#define LOCKABLESTREAMDATATEST_H

/**
 * @file LockableStreamDataTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class LockableStreamDataTest
 *
 * @brief
 *
 * @details
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

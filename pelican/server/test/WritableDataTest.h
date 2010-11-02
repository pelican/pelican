#ifndef WRITABLEDATATEST_H
#define WRITABLEDATATEST_H

/**
 * @file WritableDataTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class WritableDataTest
 *
 * @brief
 * Unit test for WritableData class
 *
 * @details
 */

class WritableDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( WritableDataTest );
        CPPUNIT_TEST( test_isValid );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_isValid();

    public:
        WritableDataTest();
        ~WritableDataTest();
};

} // namespace pelican
#endif // WRITABLEDATATEST_H

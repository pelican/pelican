#ifndef STREAMDATATEST_H
#define STREAMDATATEST_H

/**
 * @file StreamDataTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_comms
 *
 * @class StreamDataTest
 *
 * @brief
 *
 * @details
 *
 */

class StreamDataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( StreamDataTest );
        CPPUNIT_TEST( test_isValid);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_isValid();

    public:
        StreamDataTest(  );
        ~StreamDataTest();

    private:
};

} // namespace pelican
#endif // STREAMDATATEST_H

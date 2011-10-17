#ifndef CIRCULARBUFFERITERATORTEST_H
#define CIRCULARBUFFERITERATORTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file CircularBufferIteratorTest.h
 */

namespace pelican {

/**
 * @class CircularBufferIteratorTest
 *  
 * @brief
 *    Unit test for CircularBufferIteratorTest
 * @details
 * 
 */

class CircularBufferIteratorTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( CircularBufferIteratorTest );
        CPPUNIT_TEST( test_circular);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_circular();

    public:
        CircularBufferIteratorTest(  );
        ~CircularBufferIteratorTest();

    private:
};

} // namespace pelican
#endif // CIRCULARBUFFERITERATORTEST_H 

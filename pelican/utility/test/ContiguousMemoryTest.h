#ifndef CONTIGUOUSMEMORYTEST_H
#define CONTIGUOUSMEMORYTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ContiguousMemoryTest.h
 */

namespace pelican {

/**
 * @class ContiguousMemoryTest
 *  
 * @brief
 *    unit test for the contiguous memory template
 * @details
 * 
 */

class ContiguousMemoryTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ContiguousMemoryTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        ContiguousMemoryTest(  );
        ~ContiguousMemoryTest();

    private:
};

} // namespace pelican
#endif // CONTIGUOUSMEMORYTEST_H 

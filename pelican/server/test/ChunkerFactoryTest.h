#ifndef CHUNKERFACTORYTEST_H
#define CHUNKERFACTORYTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file ChunkerFactoryTest.h
 */

namespace pelican {

/**
 * @class ChunkerFactoryTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class ChunkerFactoryTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( ChunkerFactoryTest );
        CPPUNIT_TEST( test_create );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_create();

    public:
        ChunkerFactoryTest(  );
        ~ChunkerFactoryTest();

    private:
};

} // namespace pelican
#endif // CHUNKERFACTORYTEST_H 

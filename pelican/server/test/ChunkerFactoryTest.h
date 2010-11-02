#ifndef CHUNKERFACTORYTEST_H
#define CHUNKERFACTORYTEST_H

/**
 * @file ChunkerFactoryTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_server
 *
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

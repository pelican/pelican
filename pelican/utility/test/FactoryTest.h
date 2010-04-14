#ifndef FACTORYTEST_H
#define FACTORYTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file FactoryTest.h
 */

namespace pelican {

/**
 * @class FactoryTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class FactoryTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( FactoryTest );
        CPPUNIT_TEST( test_create );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_create();

    public:
        FactoryTest(  );
        ~FactoryTest();

    private:
};

} // namespace pelican
#endif // FACTORYTEST_H

#ifndef TYPECOUNTERTEST_H
#define TYPECOUNTERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file TypeCounterTest.h
 */

namespace pelican {

/**
 * @class TypeCounterTest
 *  
 * @brief
 *   unit test for the TypeCounter tmeplate
 * @details
 * 
 */

class TypeCounterTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TypeCounterTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        TypeCounterTest(  );
        ~TypeCounterTest();

    private:
};

} // namespace pelican
#endif // TYPECOUNTERTEST_H 

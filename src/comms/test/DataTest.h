#ifndef DATATEST_H
#define DATATEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataTest.h
 */

namespace pelican {

/**
 * @class DataTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class DataTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataTest );
        CPPUNIT_TEST( test_valid );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_valid();

    public:
        DataTest(  );
        ~DataTest();

    private:
};

} // namespace pelican
#endif // DATATEST_H 

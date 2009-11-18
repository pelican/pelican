#ifndef DATAREQUIREMENTSTEST_H
#define DATAREQUIREMENTSTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataRequirementsTest.h
 */

/**
 * @class DataRequirementsTest
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class DataRequirementsTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataRequirementsTest );
        CPPUNIT_TEST( test_hash );
        CPPUNIT_TEST( test_operatorEquals );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_hash();
        void test_operatorEquals();

    public:
        DataRequirementsTest(  );
        ~DataRequirementsTest();

    private:
};

} // namespace pelican
#endif // DATAREQUIREMENTSTEST_H 

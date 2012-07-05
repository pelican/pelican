#ifndef DATABLOBRELAYTEST_H
#define DATABLOBRELAYTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataBlobRelayTest.h
 */

namespace pelican {

/**
 * @class DataBlobRelayTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class DataBlobRelayTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobRelayTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        DataBlobRelayTest(  );
        ~DataBlobRelayTest();

    private:
};

} // namespace pelican
#endif // DATABLOBRELAYTEST_H 

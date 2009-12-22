#ifndef DATAMANAGERTEST_H
#define DATAMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataManagerTest.h
 */

namespace pelican {

/**
 * @class DataManagerTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class DataManagerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataManagerTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        DataManagerTest(  );
        ~DataManagerTest();

    private:
};

} // namespace pelican
#endif // DATAMANAGERTEST_H 

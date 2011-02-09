#ifndef DATAMANAGERTEST_H
#define DATAMANAGERTEST_H

/**
 * @file DataManagerTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_server
 *
 * @class DataManagerTest
 *
 * @brief
 * Unit test for the DataManager class
 *
 * @details
 */

class DataManagerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataManagerTest );
        CPPUNIT_TEST( test_getWritable );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_getWritable();

    public:
        DataManagerTest(  );
        ~DataManagerTest();

    private:
};

} // namespace pelican
#endif // DATAMANAGERTEST_H

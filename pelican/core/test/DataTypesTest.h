#ifndef DATATYPESTEST_H
#define DATATYPESTEST_H

/**
 * @file DataTypesTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_core
 *
 * @class DataTypesTest
 *
 * @brief
 *   Unit test for DataTypes class
 * @details
 *
 */

class DataTypesTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataTypesTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();

    public:
        DataTypesTest(  );
        ~DataTypesTest();

    private:
};

} // namespace pelican
#endif // DATATYPESTEST_H

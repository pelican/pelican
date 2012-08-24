#ifndef DATASPECTEST_H
#define DATASPECTEST_H

/**
 * @file DataSpecTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_data
 *
 * @class DataSpecTest
 *
 * @brief
 * Unit testing class for the data requirements object.
 *
 * @details
 * Performs unit tests on the DataSpec object
 * using the CppUnit framework.
 */

class DataSpecTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataSpecTest );
        CPPUNIT_TEST( test_hash );
        CPPUNIT_TEST( test_operator_equalTo );
        CPPUNIT_TEST( test_operator_notEqualTo );
        CPPUNIT_TEST( test_operator_additiveAssignment );
        CPPUNIT_TEST( test_operator_addition );
        CPPUNIT_TEST( test_isCompatible );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_hash();
        void test_isCompatible();
        void test_operator_equalTo();
        void test_operator_notEqualTo();
        void test_operator_additiveAssignment();
        void test_operator_addition();

    public:
        DataSpecTest();
        ~DataSpecTest();

    private:
};

} // namespace pelican
#endif // DATASPECTEST_H

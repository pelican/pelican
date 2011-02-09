#ifndef DATAREQUIREMENTSTEST_H
#define DATAREQUIREMENTSTEST_H

/**
 * @file DataRequirementsTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_data
 *
 * @class DataRequirementsTest
 *
 * @brief
 * Unit testing class for the data requirements object.
 *
 * @details
 * Performs unit tests on the DataRequirements object
 * using the CppUnit framework.
 */

class DataRequirementsTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataRequirementsTest );
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
        DataRequirementsTest();
        ~DataRequirementsTest();

    private:
};

} // namespace pelican
#endif // DATAREQUIREMENTSTEST_H

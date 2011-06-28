#ifndef DATABLOBFILETEST_H
#define DATABLOBFILETEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataBlobFileTest.h
 */

namespace pelican {

/**
 * @class DataBlobFileTest
 * 
 * @brief
 * 
 * @details
 * 
 */
class DataBlobFileTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobFileTest );
        CPPUNIT_TEST( test_heterogeneous );
        CPPUNIT_TEST( test_homogeneous );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_heterogeneous();
        void test_homogeneous();

    public:
        /// DataBlobFileTest constructor.
        DataBlobFileTest();

        /// DataBlobFileTest destructor.
        ~DataBlobFileTest();

    private:
};

} // namespace pelican

#endif // DATABLOBFILETEST_H 

#ifndef DATABLOBVERIFYTEST_H
#define DATABLOBVERIFYTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataBlobVerifyTest.h
 */

namespace pelican {
namespace test {
class TestDataBlob;
}

/**
 * @class DataBlobVerifyTest
 *  
 * @brief
 *    Test the basic functions of the DataBlobVerify class
 * @details
 * 
 */

class DataBlobVerifyTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobVerifyTest );
        CPPUNIT_TEST( test_verifySerialisedBytes );
        CPPUNIT_TEST( test_verifyDeSerialise );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_verifySerialisedBytes();
        void test_verifyDeSerialise();

    public:
        DataBlobVerifyTest(  );
        ~DataBlobVerifyTest();

    private:
       test::TestDataBlob* _testBlob;
};

} // namespace pelican
#endif // DATABLOBVERIFYTEST_H 

#ifndef DATABLOBBUFFERTEST_H
#define DATABLOBBUFFERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataBlobBufferTest.h
 */

namespace pelican {

/**
 * @class DataBlobBufferTest
 *  
 * @brief
 *    unit test for the DataBlobBuffer
 * @details
 * 
 */

class DataBlobBufferTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobBufferTest );
        CPPUNIT_TEST( test_nextMethod );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_nextMethod();

    public:
        DataBlobBufferTest(  );
        ~DataBlobBufferTest();

    private:
};

} // namespace pelican
#endif // DATABLOBBUFFERTEST_H 

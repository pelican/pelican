#ifndef DATABLOBBUFFERTEST_H
#define DATABLOBBUFFERTEST_H

#include <QtCore/QVector>
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file DataBlobBufferTest.h
 */

namespace pelican {
namespace test { class TestDataBlob; }

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
        CPPUNIT_TEST( test_shrink );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_nextMethod();
        void test_shrink();

    public:
        DataBlobBufferTest(  );
        ~DataBlobBufferTest();

    private:
        // debugging helper - print out the datablob pointers
        void dump(const QVector<test::TestDataBlob* >& blobs);
};

} // namespace pelican
#endif // DATABLOBBUFFERTEST_H 

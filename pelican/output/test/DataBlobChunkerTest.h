#ifndef DATABLOBCHUNKERTEST_H
#define DATABLOBCHUNKERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "pelican/data/test/TestDataBlob.h"

/**
 * @file DataBlobChunkerTest.h
 */

namespace pelican {

/**
 * @class DataBlobChunkerTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class DataBlobChunkerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataBlobChunkerTest );
        CPPUNIT_TEST( test_method );
        CPPUNIT_TEST( test_multisubscribe );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_method();
        void test_multisubscribe();

    public:
        DataBlobChunkerTest(  );
        ~DataBlobChunkerTest();

    private:
        test::TestDataBlob _blob;
};

} // namespace pelican
#endif // DATABLOBCHUNKERTEST_H 

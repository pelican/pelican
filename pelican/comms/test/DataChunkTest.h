#ifndef DATA_CHUNK_TEST_H
#define DATA_CHUNK_TEST_H

/**
 * @file DataChunkTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

namespace pelican {

/**
 * @ingroup t_comms
 *
 * @class DataChunkTest
 *
 * @brief
 * Testing of the Data Chunk.
 *
 * @details
 */

class DataChunkTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DataChunkTest );
        CPPUNIT_TEST( test_valid );
        CPPUNIT_TEST( test_pointer );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_valid();
        void test_pointer();

    public:
        DataChunkTest();
        ~DataChunkTest();

    private:
};

} // namespace pelican
#endif // DATA_CHUNK_TEST_H

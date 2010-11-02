#ifndef STREAMDATABUFFERTEST_H
#define STREAMDATABUFFERTEST_H

/**
 * @file StreamDataBufferTest.h
 */

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

namespace pelican {

class DataManager;

/**
 * @ingroup t_server
 *
 * @class StreamDataBufferTest
 *
 * @brief
 * Unit test for StreamDataBuffer
 *
 * @details
 */

class StreamDataBufferTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( StreamDataBufferTest );
        //CPPUNIT_TEST( test_getNext );
        //CPPUNIT_TEST( test_getWritable );
        CPPUNIT_TEST( test_getWritableStreams );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_getNext();
        void test_getWritable();
        void test_getWritableStreams();

    public:
        StreamDataBufferTest();
        ~StreamDataBufferTest();

    private:
        QCoreApplication* _app;
        DataManager* _dataManager;
};

} // namespace pelican
#endif // STREAMDATABUFFERTEST_H

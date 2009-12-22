#ifndef STREAMDATABUFFERTEST_H
#define STREAMDATABUFFERTEST_H

#include <cppunit/extensions/HelperMacros.h>
class QCoreApplication;

/**
 * @file StreamDataBufferTest.h
 */

namespace pelican {

/**
 * @class StreamDataBufferTest
 *  
 * @brief
 * 
 * @details
 * 
 */

class StreamDataBufferTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( StreamDataBufferTest );
        CPPUNIT_TEST( test_getNext );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_getNext();

    public:
        StreamDataBufferTest(  );
        ~StreamDataBufferTest();

    private:
        QCoreApplication* _app;
};

} // namespace pelican
#endif // STREAMDATABUFFERTEST_H 

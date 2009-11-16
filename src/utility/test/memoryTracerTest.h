#ifndef MEMORYTRACERTEST_H
#define MEMORYTRACERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/** Class      : memoryTracerTest
  *  
  * Description:
  *    Basic Testing of the Memory Tracer
  */

namespace pelican {

class memoryTracerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( memoryTracerTest );
        CPPUNIT_TEST( test_leak );
        CPPUNIT_TEST( test_noLeak );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_leak();
        void test_noLeak();

    public:
        memoryTracerTest(  );
        ~memoryTracerTest();

    private:
};

} // namespace pelican
#endif // MEMORYTRACERTEST_H 

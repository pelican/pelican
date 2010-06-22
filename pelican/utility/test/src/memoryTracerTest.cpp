#include "pelican/utility/test/memoryTracerTest.h"
#include "pelican/utility/memoryTracer.h"

class DummyData {
    public: 
        DummyData() {};
        ~DummyData() {};
    private:
        int data;
};

class DummyClass {
    public:
        DummyClass(bool b=false) : _leak(b) 
        {
             _data = new(__FILE__, __LINE__) DummyData;
        };
        ~DummyClass() { if(!_leak) {
                                delete _data;
                        }
        };
        DummyData* data() const { return _data; }  ;
    private:
        bool _leak;
        DummyData* _data;
};

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( memoryTracerTest );
// class memoryTracerTest 
memoryTracerTest::memoryTracerTest()
    : CppUnit::TestFixture()
{
}

memoryTracerTest::~memoryTracerTest()
{
}

void memoryTracerTest::setUp()
{
}

void memoryTracerTest::tearDown()
{
}

void memoryTracerTest::test_noLeak()
{
    DummyClass* dummy = new DummyClass();
    CPPUNIT_ASSERT_EQUAL( true, boost::details::pool::singleton_default<Tracer>::instance().entry(dummy) );
    delete dummy;
    CPPUNIT_ASSERT_EQUAL( false, boost::details::pool::singleton_default<Tracer>::instance().entry(dummy) );
}

void memoryTracerTest::test_leak()
{
    DummyClass* leaky = new DummyClass(true);
    DummyData* leak = leaky->data();
    CPPUNIT_ASSERT_EQUAL( true, boost::details::pool::singleton_default<Tracer>::instance().entry(leaky) );
    CPPUNIT_ASSERT_EQUAL( true, boost::details::pool::singleton_default<Tracer>::instance().entry(leaky) );
    delete leaky;
    CPPUNIT_ASSERT_EQUAL( true, boost::details::pool::singleton_default<Tracer>::instance().entry(leak) );
    delete leak; // clean up our test
}

} // namespace pelican

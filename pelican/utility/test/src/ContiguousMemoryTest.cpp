#include "ContiguousMemoryTest.h"
#include "ContiguousMemory.hpp"
#include <QtCore/QString>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ContiguousMemoryTest );
/**
 *@details ContiguousMemoryTest 
 */
ContiguousMemoryTest::ContiguousMemoryTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
ContiguousMemoryTest::~ContiguousMemoryTest()
{
}

void ContiguousMemoryTest::setUp()
{
}

void ContiguousMemoryTest::tearDown()
{
}

void ContiguousMemoryTest::test_method()
{
     // set a small buffer and ensure we can get values
     unsigned long max=2;
     ContiguousMemory<long> buffer(max);
     long* n1 = buffer.nextFree();
     CPPUNIT_ASSERT( n1 != (long*)NULL );
     long* n2 = buffer.nextFree();
     CPPUNIT_ASSERT( n1 != n2 );
     CPPUNIT_ASSERT( n2 != (long*)NULL );
     // buffer should now be full - expect NULL
     CPPUNIT_ASSERT_EQUAL( (long*)0, buffer.nextFree() );

     // free some space and then reallocate
     buffer.free(n2);
     long* n3 = buffer.nextFree();
     CPPUNIT_ASSERT_EQUAL( n2 , n3 );
}

} // namespace pelican

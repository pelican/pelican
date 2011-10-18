#include "LockingCircularBufferTest.h"
#include "LockingCircularBuffer.hpp"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( LockingCircularBufferTest );
/**
 *@details LockingCircularBufferTest 
 */
LockingCircularBufferTest::LockingCircularBufferTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
LockingCircularBufferTest::~LockingCircularBufferTest()
{
}

void LockingCircularBufferTest::setUp()
{
}

void LockingCircularBufferTest::tearDown()
{
}

void LockingCircularBufferTest::test_method()
{
     QList<int> buf; buf << 1 << 2 << 3 << 4;
     LockingCircularBuffer<int> buffer(&buf);
     CPPUNIT_ASSERT_EQUAL(1, buffer.next());
     CPPUNIT_ASSERT_EQUAL(2, buffer.next());
     CPPUNIT_ASSERT_EQUAL(3, buffer.next());
     CircularBufferIterator<int> it = buffer.shiftLock();
     CPPUNIT_ASSERT_EQUAL(1, *it);
     CPPUNIT_ASSERT_EQUAL(4, buffer.next());

}

} // namespace pelican

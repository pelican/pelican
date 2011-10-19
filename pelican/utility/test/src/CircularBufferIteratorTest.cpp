#include "CircularBufferIteratorTest.h"
#include "CircularBufferIterator.hpp"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( CircularBufferIteratorTest );
/**
 *@details CircularBufferIteratorTest 
 */
CircularBufferIteratorTest::CircularBufferIteratorTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
CircularBufferIteratorTest::~CircularBufferIteratorTest()
{
}

void CircularBufferIteratorTest::setUp()
{
}

void CircularBufferIteratorTest::tearDown()
{
}

void CircularBufferIteratorTest::test_circular()
{
     { // Use Case:
       // buffer size > 1
       // Expect:
       // next to return a different object for each call
       // until it reaches n, then recycle them in the same order
       QList<int> buf; buf << 1 << 2 << 3;
       pelican::CircularBufferIterator<int> it(&buf);
       CPPUNIT_ASSERT_EQUAL(1, it.next());
       CPPUNIT_ASSERT_EQUAL(1, *it);
       CPPUNIT_ASSERT_EQUAL(2, it.peekNext());
       CPPUNIT_ASSERT_EQUAL(2, it.next());
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       CPPUNIT_ASSERT_EQUAL(1, it.peekNext());
       CPPUNIT_ASSERT_EQUAL(1, it.next());
       CPPUNIT_ASSERT_EQUAL(2, it.next());
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       CPPUNIT_ASSERT_EQUAL(1, it.next());
       CPPUNIT_ASSERT_EQUAL(2, it.next());
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       // test resetIndex - existing value
       it.resetIndex(2);
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       // test resetIndex - non-existing value
       // expect to reset to beginning
       it.resetIndex(6);
       CPPUNIT_ASSERT_EQUAL(1, it.next());
     }
}

} // namespace pelican

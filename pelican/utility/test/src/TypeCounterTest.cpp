#include "TypeCounterTest.h"
#include "TypeCounter.h"


namespace pelican {
CPPUNIT_TEST_SUITE_REGISTRATION( TypeCounterTest );

/**
 *@details TypeCounterTest 
 */
TypeCounterTest::TypeCounterTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
TypeCounterTest::~TypeCounterTest()
{
}

void TypeCounterTest::setUp()
{
}

void TypeCounterTest::tearDown()
{
}

void TypeCounterTest::test_method()
{
     TypeCounter<int> c;
     c.add(5);
     CPPUNIT_ASSERT_EQUAL(5, c.max());
     CPPUNIT_ASSERT_EQUAL(5, c.min());
     c.add(7);
     CPPUNIT_ASSERT_EQUAL(7, c.max());
     CPPUNIT_ASSERT_EQUAL(5, c.min());
     c.add(77);
     CPPUNIT_ASSERT_EQUAL(77, c.max());
     CPPUNIT_ASSERT_EQUAL(5, c.min());
     c.remove(77);
     CPPUNIT_ASSERT_EQUAL(7, c.max());
     CPPUNIT_ASSERT_EQUAL(5, c.min());
     c.add(100);
     CPPUNIT_ASSERT_EQUAL(100, c.max());
     CPPUNIT_ASSERT_EQUAL(5, c.min());
     c.add(1);
     CPPUNIT_ASSERT_EQUAL(100, c.max());
     CPPUNIT_ASSERT_EQUAL(1, c.min());
     c.remove(7); // remove an intermediate
     CPPUNIT_ASSERT_EQUAL(100, c.max());
     CPPUNIT_ASSERT_EQUAL(1, c.min());
     c.remove(1); // remove lowest
     CPPUNIT_ASSERT_EQUAL(100, c.max());
     CPPUNIT_ASSERT_EQUAL(5, c.min());
}

} // namespace pelican

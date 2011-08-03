#include "WritableDataTest.h"
#include "WritableData.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( WritableDataTest );
// class WritableDataTest
WritableDataTest::WritableDataTest()
    : CppUnit::TestFixture()
{
}

WritableDataTest::~WritableDataTest()
{
}

void WritableDataTest::setUp()
{
}

void WritableDataTest::tearDown()
{
}

void WritableDataTest::test_isValid()
{
    {
        // Use Case:
        // Data reference is a null pointer
        // expect return invalid
        WritableData wd(0);
        CPPUNIT_ASSERT( ! wd.isValid() );
    }
}

} // namespace pelican

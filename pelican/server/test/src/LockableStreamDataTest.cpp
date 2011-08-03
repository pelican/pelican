#include "pelican/server/test/LockableStreamDataTest.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/LockableServiceData.h"
#include "pelican/server/LockedData.h"
#include "pelican/comms/StreamData.h"
#include <QtCore/QString>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( LockableStreamDataTest );
/**
 *@details LockableStreamDataTest 
 */
LockableStreamDataTest::LockableStreamDataTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
LockableStreamDataTest::~LockableStreamDataTest()
{
}

void LockableStreamDataTest::setUp()
{
}

void LockableStreamDataTest::tearDown()
{
}

void LockableStreamDataTest::test_addAssociates()
{
    std::vector<char> data(10);
    QString name("locked1");
    LockableServiceData ld(name,&data[0],data.size()); // dummy associate data
    LockedData lockedData(name, &ld);   // lock the dummy
    LockableStreamData d("test", &data[0], data.size()); // create test object
    CPPUNIT_ASSERT_EQUAL( true, d.isValid() );

    // Use Case:
    // Add a single associuate
    // Expect local copy and underlying StreamData to 
    // have the associate
    CPPUNIT_ASSERT_EQUAL( 0, (int)d.associateData().size() );
    CPPUNIT_ASSERT_EQUAL( 0, (int)d.streamData()->associateData().size() );
    d.addAssociatedData(lockedData);
    CPPUNIT_ASSERT_EQUAL( 1, (int)d.associateData().size() );
    CPPUNIT_ASSERT_EQUAL( 1, (int)d.streamData()->associateData().size() );
    CPPUNIT_ASSERT_EQUAL( true, d.isValid() );

    // Use Case:
    // reset(0) should empty the associatedData
    // of both lockedData and its underlying StreamData
    // object, object should not be valid
    d.reset(0);
    CPPUNIT_ASSERT_EQUAL( 0, (int)d.associateData().size() );
    CPPUNIT_ASSERT_EQUAL( 0, (int)d.streamData()->associateData().size() );
    CPPUNIT_ASSERT_EQUAL( false, d.isValid() );
}


} // namespace pelican

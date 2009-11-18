#include "DataRequirementsTest.h"
#include "DataRequirements.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataRequirementsTest );
// class DataRequirementsTest 
DataRequirementsTest::DataRequirementsTest()
    : CppUnit::TestFixture()
{
}

DataRequirementsTest::~DataRequirementsTest()
{
}

void DataRequirementsTest::setUp()
{
}

void DataRequirementsTest::tearDown()
{
}

void DataRequirementsTest::test_hash()
{
    DataRequirements d;
    d.setStreamData(QString("tele"));

    // Use Case:
    // Ensure hashing variable returns a non-zero value
    {
        CPPUNIT_ASSERT(d.hash() != 0);
    }

    // Use Case:
    // Ensure different DataRequirement objects return different hash values
    {
        DataRequirements d2;
        d2.setStreamData(QString("wibble"));
        CPPUNIT_ASSERT(d2.hash() != d.hash());
    }

    // Use Case:
    // Ensure different ordering returns the same hash
    {
        DataRequirements d1;
        QStringList list1;
        list1.append("wibble1");
        list1.append("wibble2");
        d1.setStreamData(list1);

        DataRequirements d2;
        QStringList list2;
        list2.append("wibble2");
        list2.append("wibble1");
        d2.setStreamData(list2);
        
        CPPUNIT_ASSERT(d1.hash() == d2.hash());
    }
}

void DataRequirementsTest::test_operatorEquals()
{
    // Use Case:
    // Test for identical objects.
    {
        DataRequirements d;
        d.setStreamData(QString("tele"));
        DataRequirements d2;
        d2.setStreamData(QString("tele"));
        CPPUNIT_ASSERT(d == d2);
    }
}

} // namespace pelican

#include "DataRequirementsTest.h"
#include "DataRequirements.h"
#include <QSet>
#include <QStringList>

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
    // Use Case:
    // Ensure hash returns a non-zero value.
    {
        DataRequirements d;
        d.setStreamData(QString("hello"));
        CPPUNIT_ASSERT(d.hash() != 0);
    }

    // Use Case:
    // Ensure different DataRequirement objects return different hash values.
    {
        DataRequirements d1;
        d1.setStreamData(QString("one"));
        DataRequirements d2;
        d2.setStreamData(QString("two"));
        CPPUNIT_ASSERT(d1.hash() != d2.hash());
    }

    // Use Case:
    // Ensure different ordering returns the same hash.
    {
        DataRequirements d1;
        QSet<QString> list1;
        list1 << "one" << "two";
        d1.setStreamData(list1);

        DataRequirements d2;
        QSet<QString> list2;
        list2 << "two" << "one";
        d2.setStreamData(list2);
        CPPUNIT_ASSERT(d1.hash() == d2.hash());
    }
}

void DataRequirementsTest::test_operators()
{
    // Use Case:
    // Test for identical objects.
    {
        DataRequirements d1;
        d1.setStreamData(QString("hello"));
        DataRequirements d2;
        d2.setStreamData(QString("hello"));
        CPPUNIT_ASSERT(d1 == d2);
    }

    // Use Case:
    // Test for merging two requirements objects.
    {
        DataRequirements d1, d2, d3;
        QStringList req;
        req << "one" << "two";

        d1.setStreamData(req.at(0));
        d2.setStreamData(req.at(1));
        d3.setStreamData(req.toSet());
        d1 += d2;
        CPPUNIT_ASSERT(d1 == d3);
    }

    // Use Case:
    // Test for merging two requirements objects in a different order.
    {
        DataRequirements d1, d2, d3;
        QStringList req;
        req << "one" << "two";

        d1.setStreamData(req.at(0));
        d2.setStreamData(req.at(1));
        d3.setStreamData(req.toSet());
        d2 += d1;
        CPPUNIT_ASSERT(d2 == d3);
    }

    // Use Case:
    // Test for merging two requirements objects, removing duplicates.
    {
        DataRequirements d, d1, d2;
        QSet<QString> req, req1, req2;
        req  << "one" << "two" << "three";
        req1 << "one" << "two";
        req2 << "two" << "three";

        d.setStreamData(req);
        d1.setStreamData(req1);
        d2.setStreamData(req2);
        d2 += d1;
        CPPUNIT_ASSERT(d == d2);
    }
}

} // namespace pelican

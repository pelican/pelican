#include "pelican/data/test/DataRequirementsTest.h"
#include "pelican/data/DataRequirements.h"
#include <QtCore/QSet>
#include <QtCore/QStringList>

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
        d.addStreamData(QString("hello"));
        CPPUNIT_ASSERT(d.hash() != 0);
    }

    // Use Case:
    // Ensure different DataRequirement objects return different hash values.
    {
        DataRequirements d1;
        d1.addStreamData(QString("one"));
        DataRequirements d2;
        d2.addStreamData(QString("two"));
        CPPUNIT_ASSERT(d1.hash() != d2.hash());
    }

    // Use Case:
    // Ensure different ordering returns the same hash.
    {
        DataRequirements d1;
        QSet<QString> list1;
        list1 << "one" << "two";
        d1.addStreamData(list1);

        DataRequirements d2;
        QSet<QString> list2;
        list2 << "two" << "one";
        d2.addStreamData(list2);
        CPPUNIT_ASSERT(d1.hash() == d2.hash());
    }
}

void DataRequirementsTest::test_isCompatible()
{
    // Use Case:
    // DataRequirements matches the data hash
    // expect true
    {
        DataRequirements req;
        req.addServiceData("wibble1");
        QHash<QString, DataBlob*> hashData;
        hashData.insert("wibble1", NULL);
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(hashData));

        DataRequirements reqdata;
        reqdata.addServiceData("wibble1");
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(reqdata));
    }

    // Use Case:
    // DataRequirements contains a subset of the data hash
    // expect true
    {
        DataRequirements req;
        req.addServiceData("wibble1");
        QHash<QString, DataBlob*> hashData;
        hashData.insert("wibble1", NULL);
        hashData.insert("wibble2", NULL);
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(hashData));

        DataRequirements reqdata;
        reqdata.addServiceData("wibble1");
        reqdata.addServiceData("wibble2");
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(reqdata));
    }

    // Use Case:
    // DataRequirements contains some elements of the data, but is not compatible with it.
    // The data requirements have an additional data type which is not in the hash.
    // expect false
    {
        DataRequirements req;
        QSet<QString> dataTypes;
        dataTypes << "wibble1" << "other";
        req.addServiceData(dataTypes);
        QHash<QString, DataBlob*> hashData;
        hashData.insert("wibble1", NULL);
        hashData.insert("wibble2", NULL);
        CPPUNIT_ASSERT_EQUAL(false, req.isCompatible(hashData));

        DataRequirements reqdata;
        reqdata.addServiceData("wibble1");
        reqdata.addServiceData("wibble2");
        CPPUNIT_ASSERT_EQUAL(false, req.isCompatible(reqdata));
    }

}

void DataRequirementsTest::test_operator_equalTo()
{
    // Use Case:
    // Test for identical objects.
    {
        DataRequirements d1;
        d1.addStreamData(QString("hello"));
        DataRequirements d2;
        d2.addStreamData(QString("hello"));
        CPPUNIT_ASSERT(d1 == d2);
    }
}

void DataRequirementsTest::test_operator_notEqualTo()
{
    // Use Case:
    // Test for non identical objects.
    {
        DataRequirements d1;
        d1.addStreamData(QString("hello1"));
        DataRequirements d2;
        d2.addStreamData(QString("hello2"));
        CPPUNIT_ASSERT(d1 != d2);
    }
}

void DataRequirementsTest::test_operator_additiveAssignment()
{
    // Use Case:
    // Test for merging two requirements objects.
    {
        DataRequirements d1, d2, d3;
        QStringList req;
        req << "one" << "two";

        d1.addStreamData(req.at(0));
        d2.addStreamData(req.at(1));
        d3.addStreamData(req.toSet());
        d1 += d2;
        CPPUNIT_ASSERT(d1 == d3);
    }

    // Use Case:
    // Test for merging two requirements objects in a different order.
    {
        DataRequirements d1, d2, d3;
        QStringList req;
        req << "one" << "two";

        d1.addStreamData(req.at(0));
        d2.addStreamData(req.at(1));
        d3.addStreamData(req.toSet());
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

        d.addStreamData(req);
        d1.addStreamData(req1);
        d2.addStreamData(req2);
        d2 += d1;
        CPPUNIT_ASSERT(d == d2);
    }
}

void DataRequirementsTest::test_operator_addition()
{
    // Use Case:
    // Test for adding two objects together.
    {
        QSet<QString> l1, l2, l3, l4;
        l1 << "one" << "two";
        l2 << "three" << "four";
        l3 << "five" << "six";
        l4 << "seven" << "eight";

        DataRequirements d1, d2, d3, d4;
        d1.addStreamData(l1);
        d1.addServiceData(l2);
        d2.addStreamData(l3);
        d2.addServiceData(l4);

        // Add the two objects together.
        d3 = d1 + d2;

        d4.addStreamData(l1);
        d4.addStreamData(l3);
        d4.addServiceData(l2);
        d4.addServiceData(l4);
        CPPUNIT_ASSERT(d3 == d4);
    }
}

} // namespace pelican

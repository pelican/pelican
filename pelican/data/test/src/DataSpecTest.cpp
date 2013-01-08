#include "data/test/DataSpecTest.h"
#include "data/DataSpec.h"
#include <QtCore/QSet>
#include <QtCore/QStringList>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataSpecTest );
// class DataSpecTest
DataSpecTest::DataSpecTest()
    : CppUnit::TestFixture()
{
}

DataSpecTest::~DataSpecTest()
{
}

void DataSpecTest::setUp()
{
}

void DataSpecTest::tearDown()
{
}

void DataSpecTest::test_hash()
{
    // Use Case:
    // Ensure hash returns a non-zero value.
    {
        DataSpec d;
        d.addStreamData(QString("hello"));
        CPPUNIT_ASSERT(d.hash() != 0);
    }

    // Use Case:
    // Ensure different DataRequirement objects return different hash values.
    {
        DataSpec d1;
        d1.addStreamData(QString("one"));
        DataSpec d2;
        d2.addStreamData(QString("two"));
        CPPUNIT_ASSERT(d1.hash() != d2.hash());
    }

    // Use Case:
    // Ensure different ordering returns the same hash.
    {
        DataSpec d1;
        QSet<QString> list1;
        list1 << "one" << "two";
        d1.addStreamData(list1);

        DataSpec d2;
        QSet<QString> list2;
        list2 << "two" << "one";
        d2.addStreamData(list2);
        CPPUNIT_ASSERT(d1.hash() == d2.hash());
    }
}

void DataSpecTest::test_isCompatible()
{
    // Use Case:
    // DataSpec matches the data hash
    // expect true
    {
        DataSpec req;
        req.addServiceData("wibble1");
        QHash<QString, DataBlob*> hashData;
        hashData.insert("wibble1", NULL);
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(hashData));

        DataSpec reqdata;
        reqdata.addServiceData("wibble1");
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(reqdata));
    }

    // Use Case:
    // DataSpec contains a subset of the data hash
    // expect true
    {
        DataSpec req;
        req.addServiceData("wibble1");
        QHash<QString, DataBlob*> hashData;
        hashData.insert("wibble1", NULL);
        hashData.insert("wibble2", NULL);
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(hashData));

        DataSpec reqdata;
        reqdata.addServiceData("wibble1");
        reqdata.addServiceData("wibble2");
        CPPUNIT_ASSERT_EQUAL(true, req.isCompatible(reqdata));
    }

    // Use Case:
    // DataSpec contains some elements of the data, but is not compatible with it.
    // The data requirements have an additional data type which is not in the hash.
    // expect false
    {
        DataSpec req;
        QSet<QString> dataTypes;
        dataTypes << "wibble1" << "other";
        req.addServiceData(dataTypes);
        QHash<QString, DataBlob*> hashData;
        hashData.insert("wibble1", NULL);
        hashData.insert("wibble2", NULL);
        CPPUNIT_ASSERT_EQUAL(false, req.isCompatible(hashData));

        DataSpec reqdata;
        reqdata.addServiceData("wibble1");
        reqdata.addServiceData("wibble2");
        CPPUNIT_ASSERT_EQUAL(false, req.isCompatible(reqdata));
    }

}

void DataSpecTest::test_operator_equalTo()
{
    // Use Case:
    // Test for identical objects.
    {
        DataSpec d1;
        d1.addStreamData(QString("hello"));
        DataSpec d2;
        d2.addStreamData(QString("hello"));
        CPPUNIT_ASSERT(d1 == d2);
    }
}

void DataSpecTest::test_operator_notEqualTo()
{
    // Use Case:
    // Test for non identical objects.
    {
        DataSpec d1;
        d1.addStreamData(QString("hello1"));
        DataSpec d2;
        d2.addStreamData(QString("hello2"));
        CPPUNIT_ASSERT(d1 != d2);
    }
}

void DataSpecTest::test_operator_additiveAssignment()
{
    // Use Case:
    // Test for merging two requirements objects.
    {
        DataSpec d1, d2, d3;
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
        DataSpec d1, d2, d3;
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
        DataSpec d, d1, d2;
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

void DataSpecTest::test_operator_addition()
{
    // Use Case:
    // Test for adding two objects together.
    {
        QSet<QString> l1, l2, l3, l4;
        l1 << "one" << "two";
        l2 << "three" << "four";
        l3 << "five" << "six";
        l4 << "seven" << "eight";

        DataSpec d1, d2, d3, d4;
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

#include <QtCore/QFile>
#include "DataBlobFileTest.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/data/test/TestDataBlob.h"
#include "pelican/utility/test/TestFile.h"
#include "DataBlobFile.h"
#include "DataBlobFileReader.h"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobFileTest );
/**
 * @details Constructs a DataBlobFileTest object.
 */
DataBlobFileTest::DataBlobFileTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the DataBlobFileTest object.
 */
DataBlobFileTest::~DataBlobFileTest()
{
}

void DataBlobFileTest::setUp()
{
}

void DataBlobFileTest::tearDown()
{
}

void DataBlobFileTest::test_homogeneous()
{
    try {
    QString stream = "blobs";
    test::TestDataBlob blob1;
    test::TestDataBlob blob2;
    // some datablobs of a different type
    test::TestDataBlob blob3("TestDataBlobType2");
    {
        // Use Case:
        // Write out a homogeneous file
        // Write multiple objects of same type
        // Expect:
        // Readable File with objects correctly recovered
        // with the Reader class
        test::TestFile file;
        QString filename = file.filename();
        ConfigNode config;
        {
            DataBlobFile writer(config);
            writer.addFile(filename, DataBlobFileType::Homogeneous );
            writer.send(stream, &blob1);
            writer.send(stream, &blob2);
        }
        CPPUNIT_ASSERT( QFile::exists(filename) );
        DataBlobFileReader r;
        r.open(filename);
        CPPUNIT_ASSERT_EQUAL( blob1.type().toStdString(),
                               r.nextBlob().toStdString() );
        test::TestDataBlob ref1;
        r.readData( &ref1 );
        CPPUNIT_ASSERT_EQUAL( blob1.type().toStdString(),
                               r.nextBlob().toStdString() );
        test::TestDataBlob ref2;
        r.readData( &ref2 );
        CPPUNIT_ASSERT_EQUAL( std::string(""),  r.nextBlob().toStdString() ); // no more blobs
    }
    {
        // Use Case:
        // Write out a homogeneous file
        // Write multiple objects of different type
        // Expect:
        // Only objects corresponding to the same type
        // as the first DataBlob should be saved
        test::TestFile file;
        QString filename = file.filename();
        ConfigNode config;
        {
            DataBlobFile writer(config);
            writer.setVerbose(2);
            writer.addFile(filename, DataBlobFileType::Homogeneous);
            writer.send(stream, &blob1);
            writer.send(stream, &blob3);
        }
        CPPUNIT_ASSERT( QFile::exists(filename) );
        DataBlobFileReader r;
        r.open(filename);
        CPPUNIT_ASSERT_EQUAL( blob1.type().toStdString(),
                               r.nextBlob().toStdString() );
        test::TestDataBlob ref1;
        r.readData( &ref1 );
        CPPUNIT_ASSERT_EQUAL( std::string("") ,  r.nextBlob().toStdString() ); // no more blobs
    }
    } catch ( const QString& e ) {
        CPPUNIT_FAIL(e.toStdString());
    }
}

void DataBlobFileTest::test_heterogeneous()
{
    try {
    QString stream = "blobs";
    test::TestDataBlob blob1;
    test::TestDataBlob blob2("TestDataType2");
    // some datablobs of a different type
    test::TestDataBlob blob3("TestDataType3");
    {
        // Use Case:
        // Write out a homogeneous file
        // Write multiple objects of same type
        // Expect:
        // Readable File with objects correctly recovered
        // with the Reader class
        test::TestFile file;
        QString filename = file.filename();
        ConfigNode config;
        {
            DataBlobFile writer(config);
            writer.addFile(filename, DataBlobFileType::Heterogeneous);
            writer.send(stream, &blob1);
            writer.send(stream, &blob2);
            writer.send(stream, &blob3);
        }
        CPPUNIT_ASSERT( QFile::exists(filename) );
        DataBlobFileReader r;
        r.open(filename);
        CPPUNIT_ASSERT_EQUAL( blob1.type().toStdString(),
                               r.nextBlob().toStdString() );
        test::TestDataBlob ref1;
        r.readData( &ref1 );
        CPPUNIT_ASSERT_EQUAL( blob2.type().toStdString(),
                               r.nextBlob().toStdString() );
        test::TestDataBlob ref2("TestDataType2");
        r.readData( &ref2 );
        CPPUNIT_ASSERT_EQUAL( blob3.type().toStdString(),
                               r.nextBlob().toStdString() );
        test::TestDataBlob ref3("TestDataType3");
        r.readData( &ref3 );
        CPPUNIT_ASSERT_EQUAL( std::string(""),  r.nextBlob().toStdString() ); // no more blobs
    }
    } catch ( const QString& e ) {
        CPPUNIT_FAIL(e.toStdString());
    }
}

} // namespace pelican

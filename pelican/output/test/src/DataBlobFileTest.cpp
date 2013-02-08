/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <QtCore/QFile>
#include "DataBlobFileTest.h"
#include "utility/ConfigNode.h"
#include "data/test/TestDataBlob.h"
#include "utility/test/TestFile.h"
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

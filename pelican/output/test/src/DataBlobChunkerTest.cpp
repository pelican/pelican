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

#include "DataBlobChunkerTest.h"
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpSocket>
#include <QtTest/QSignalSpy>
#include "server/test/ChunkerTester.h"
#include "output/test/TestDataBlobServer.h"
#include <QtCore/QDebug>


namespace pelican {
using namespace test;

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobChunkerTest );
/**
 *@details DataBlobChunkerTest
 */
DataBlobChunkerTest::DataBlobChunkerTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataBlobChunkerTest::~DataBlobChunkerTest()
{
}

void DataBlobChunkerTest::setUp()
{
    _blob.setData( QByteArray("testdata") );
}

void DataBlobChunkerTest::tearDown()
{
}

void DataBlobChunkerTest::test_method()
{
    // Use Case:
    // Send datablobs on a single stream
    // Expect:
    // individual requests for buffer space
    // for each DataBlob
    try {
    TestDataBlobServer server;
    QString xml("<DataBlobChunker>");
    xml += QString("<connection host=\"%1\" port=\"%2\" />")
           .arg(server.host()).arg(server.port());
    xml += QString("<subscribe stream=\"%1\" />").arg(_blob.type());
    xml += "</DataBlobChunker>";
    CPPUNIT_ASSERT_EQUAL( 0, server.clientsForStream(_blob.type()) );
    ChunkerTester tester("DataBlobChunker", 10*_blob.size(), xml );
    CPPUNIT_ASSERT_EQUAL( 1, server.clientsForStream(_blob.type()) );
    server.send(&_blob);
    CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount() );
    server.send(&_blob);
    CPPUNIT_ASSERT_EQUAL( 2, tester.writeRequestCount() );
    } catch(const QString& e) {
        CPPUNIT_FAIL(e.toStdString().c_str());
    }
}

void DataBlobChunkerTest::test_multisubscribe() {
    // Use Case:
    // Send datablobs on multiple streams
    // Expect:
    // individual requests for buffer space
    // for each DataBlob - different buffers for
    // each stream
    TestDataBlobServer server;
    QString stream1("stream1");
    QString stream2("stream2");
    QString xml("<DataBlobChunker>");
    xml += QString("<connection host=\"%1\" port=\"%2\" />")
           .arg(server.host()).arg(server.port());
    xml += QString("<subscribe stream=\"%1\" />").arg(stream1);
    xml += QString("<subscribe stream=\"%1\" />").arg(stream2);
    xml += "</DataBlobChunker>";
    QList<QString> list; list << stream1 << stream2;
    ChunkerTester tester("DataBlobChunker", 10*_blob.size(), xml );
    CPPUNIT_ASSERT_EQUAL( 1, server.clientsForStream(stream1) );
    CPPUNIT_ASSERT_EQUAL( 1, server.clientsForStream(stream2) );
    server.send(&_blob, stream1);
    CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount(stream1) );
    server.send(&_blob, stream2);
    CPPUNIT_ASSERT_EQUAL( 1, tester.writeRequestCount(stream2) );
}

} // namespace pelican

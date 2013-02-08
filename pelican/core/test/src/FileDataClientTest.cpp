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

#include "FileDataClientTest.h"
#include "FileDataClient.h"

#include "utility/Config.h"
#include "utility/ConfigNode.h"
#include "data/DataSpec.h"
#include "data/DataBlob.h"
#include "core/DataTypes.h"
#include "core/test/TestStreamAdapter.h"

namespace pelican {
using test::TestStreamAdapter;

CPPUNIT_TEST_SUITE_REGISTRATION( FileDataClientTest );
/**
 * @details Constructs a FileDataClientTest object.
 */
FileDataClientTest::FileDataClientTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the FileDataClientTest object.
 */
FileDataClientTest::~FileDataClientTest()
{
}

void FileDataClientTest::setUp()
{
}

void FileDataClientTest::tearDown()
{
}

void FileDataClientTest::test_factory()
{
    // Use case:
    // Instantiate with a factory
    try {
    Config config;
    config.setFromString(
            "<adapters>"
            "   <file name=\"testfile.xml\"/>"
            "</adapters>"
            );
    Config::TreeAddress address;
    address << Config::NodeId("adapters", "");
    ConfigNode configNode = config.get(address);
    }
    catch( QString& e ) {
        CPPUNIT_FAIL(e.toStdString());
    }

}

void FileDataClientTest::test_method()
{
    try {
    Config config;
    config.setFromString(
            "<testconfig>"
            "   <file name=\"testfile.xml\"/>"
            "</testconfig>"
            );
    Config::TreeAddress address;
    address << Config::NodeId("testconfig", "");
    ConfigNode configNode = config.get(address);
/* No longer relevant
    {
        // Use Case:
        //    Empty data requirements
        // Expect: throw with a suitable complaint

        // setup the test
        QList<DataSpec> lreq;
        DataTypes dt;
        dt.addData(lreq);
        FileDataClient* client = 0;
        CPPUNIT_ASSERT_THROW(client = new FileDataClient(configNode, dt, 0), QString);
        delete client;
    }
*/
    TestStreamAdapter streamAdapter;
    QString stream1("stream1");
    QString version1("version1");
    DataSpec req;
    req.addStreamData(stream1);
    {
        // Use Case:
        //    data requirements are set, empty hash passed
        // Expect: throw

        QList<DataSpec> lreq;
        lreq.append(req);
        DataTypes dt;
        dt.setAdapter(stream1,&streamAdapter);
        dt.addData(lreq);
        FileDataClient client(configNode, dt, 0);

        QHash<QString, DataBlob*> dataHash;
        CPPUNIT_ASSERT_THROW(client.getData(dataHash), QString );
    }
    {  // Use Case:
       // Specifiy a single file and associate it with a Stream
       // correct hash is passed
       // Expect:
       // Data in file to be streamed
       DataTypes types;
       types.setAdapter(stream1,&streamAdapter);
       types.addData(req);
       FileDataClient client(configNode, types, &config);

       QHash<QString, DataBlob*> dataHash;
       DataBlob db("DataBlob");
       dataHash.insert(stream1, &db);
       client.getData(dataHash);
    }
    }
    catch( QString& e ) {
        CPPUNIT_FAIL(e.toStdString());
    }
}

} // namespace pelican

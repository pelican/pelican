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

#include "DataBlobRelayTest.h"
#include "DataBlobRelay.h"
#include "TestDataBlobClient.h"
#include "TestOutputStreamer.h"
#include "Stream.h"
#include "utility/Config.h"
#include "data/test/TestDataBlob.h"
#include <boost/shared_ptr.hpp>


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobRelayTest );
/**
 *@details DataBlobRelayTest
 */
DataBlobRelayTest::DataBlobRelayTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataBlobRelayTest::~DataBlobRelayTest()
{
}

void DataBlobRelayTest::setUp()
{
}

void DataBlobRelayTest::tearDown()
{
}

void DataBlobRelayTest::test_method()
{
     Config config;
     QString streamId("testStream");
     {
         // Use Case: empty config
         // Expect: no issues constructing object
         Config::TreeAddress address;
         DataBlobRelay r( &config, address );

        // add a client
        test::TestDataBlobClient client;
        r.addClient( &client );

        // add a Streamer
        test::TestOutputStreamer testStreamer;
        r.connectToStream( &testStreamer, streamId );

        // check data flow
        boost::shared_ptr<DataBlob> testData( new test::TestDataBlob );
        Stream stream(streamId);
        stream.setData( testData );
        CPPUNIT_ASSERT_EQUAL( (void*)0, (void*)testStreamer.lastReceived( streamId ));
        client.send( stream );
        CPPUNIT_ASSERT_EQUAL( (void*)testData.get(), (void*)testStreamer.lastReceived( streamId ));
     }
}

} // namespace pelican

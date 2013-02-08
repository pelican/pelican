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

#include "comms/test/StreamDataTest.h"

#include "comms/StreamData.h"
#include "comms/DataChunk.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( StreamDataTest );
// class StreamDataTest
StreamDataTest::StreamDataTest()
    : CppUnit::TestFixture()
{
}

StreamDataTest::~StreamDataTest()
{
}

void StreamDataTest::setUp()
{
}

void StreamDataTest::tearDown()
{
}

void StreamDataTest::test_isValid()
{
    {
        // Use Case:
        // Stream Data invalid, no associated data
        // expect invalid
        StreamData sd("",(void*)0,10);
        CPPUNIT_ASSERT( ! sd.isValid() );
    }
    {
        // Use Case:
        // Stream Data valid, no associated data
        // expect valid
        StreamData sd("",(void*)1000,10);
        CPPUNIT_ASSERT( sd.isValid() );
    }
    {
        // Use Case:
        // Stream Data is valid, but associate Data is not
        // expect invalid
        DataChunk d;
        {
            StreamData sd("",(void*)1000,10);
            CPPUNIT_ASSERT( ! d.isValid() );
            CPPUNIT_ASSERT( sd.isValid() );
            boost::shared_ptr<DataChunk> ld( new DataChunk("test", &d) );
            sd.addAssociatedData( ld );
            CPPUNIT_ASSERT( ! sd.isValid() );
        }
    }
}

} // namespace pelican

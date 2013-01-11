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

#include "core/test/DataTypesTest.h"
#include "core/DataTypes.h"
#include "core/test/TestServiceAdapter.h"
#include "core/test/TestStreamAdapter.h"
#include "data/DataSpec.h"

namespace pelican {

using test::TestServiceAdapter;
using test::TestStreamAdapter;

CPPUNIT_TEST_SUITE_REGISTRATION( DataTypesTest );
/**
 *@details DataTypesTest
 */
DataTypesTest::DataTypesTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataTypesTest::~DataTypesTest()
{
}

void DataTypesTest::setUp()
{
}

void DataTypesTest::tearDown()
{
}

void DataTypesTest::test_method()
{
    TestServiceAdapter service_adapter;
    TestStreamAdapter stream_adapter;
    {
        // Use case:
        // Single DataRequirement
        // Adapter is of the correct type
        // Expect corresponding dataReq to be the same
        DataSpec req;
        req.addServiceData("service1");
        req.addStreamData("stream1");
        DataTypes dt;
        dt.setAdapter("service1", &service_adapter);
        dt.setAdapter("stream1", &stream_adapter);
        dt.addData(req);
        CPPUNIT_ASSERT_EQUAL(1, dt.dataSpec().size() );
        DataSpec out = dt.dataSpec()[0];
        CPPUNIT_ASSERT( req.streamData().values()[0] == out.streamData().values()[0] );
        CPPUNIT_ASSERT( req.serviceData().values()[0] == out.serviceData().values()[0] );
    }
    {
        // Use case:
        // Single DataRequirement
        // Adapter is of a different type
        // Expect corresponding dataReq to be adjusted to adpater
        DataSpec req;
        req.addStreamData("service1");
        req.addServiceData("stream1");
        DataTypes dt;
        dt.setAdapter("service1", &service_adapter);
        dt.setAdapter("stream1", &stream_adapter);
        dt.addData(req);
        CPPUNIT_ASSERT_EQUAL(1, dt.dataSpec().size() );
        const DataSpec& out = dt.dataSpec()[0];
        CPPUNIT_ASSERT( (req.streamData().values()[0]) == (out.serviceData().values()[0]) );
        CPPUNIT_ASSERT( req.serviceData().values()[0] == out.streamData().values()[0] );
    }
}

} // namespace pelican

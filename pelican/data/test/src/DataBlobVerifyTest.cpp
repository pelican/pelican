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

#include "DataBlobVerifyTest.h"
#include "DataBlobVerify.h"
#include "data/test/TestDataBlob.h"
#include <QtCore/QDebug>

namespace pelican {
namespace test {
class BrokenTestDataBlob: public test::TestDataBlob {
    public:
        BrokenTestDataBlob() : TestDataBlob("BrokenTestDataBlob") {}
        ~BrokenTestDataBlob() {}
        virtual quint64 serialisedBytes() const {
            return 2*TestDataBlob::serialisedBytes(); }
        virtual void deserialise(QIODevice&, QSysInfo::Endian ) {
            data()="some rubbish";
        };
};
PELICAN_DECLARE_DATABLOB(BrokenTestDataBlob)
} // namespace test


CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobVerifyTest );
/**
 *@details DataBlobVerifyTest
 */
DataBlobVerifyTest::DataBlobVerifyTest()
    : CppUnit::TestFixture(), _testBlob(0)
{
}

/**
 *@details
 */
DataBlobVerifyTest::~DataBlobVerifyTest()
{
}

void DataBlobVerifyTest::setUp()
{
    _testBlob = new test::TestDataBlob;
    _testBlob->setData("abc");
}

void DataBlobVerifyTest::tearDown()
{
    delete _testBlob;
}

void DataBlobVerifyTest::test_verifySerialisedBytes()
{
    DataBlobVerify dbv( _testBlob );
    CPPUNIT_ASSERT(dbv.verifySerialisedBytes());

    test::BrokenTestDataBlob badBlob;
    badBlob.setData("abc");
    DataBlobVerify vb( &badBlob );
    CPPUNIT_ASSERT( ! vb.verifySerialisedBytes());
}

void DataBlobVerifyTest::test_verifyDeSerialise()
{
    DataBlobVerify dbv( _testBlob );
    CPPUNIT_ASSERT(dbv.verifyDeserialise());

    test::BrokenTestDataBlob badBlob;
    badBlob.setData("abc");
    DataBlobVerify vb( &badBlob );
    CPPUNIT_ASSERT( ! vb.verifyDeserialise());
}

} // namespace pelican

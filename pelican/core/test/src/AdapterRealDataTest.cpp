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

#include "AdapterRealDataTest.h"
#include "core/AdapterRealData.h"
#include "data/ArrayData.h"
#include "utility/ConfigNode.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QBuffer>

#include <iostream>
#include <iomanip>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AdapterRealDataTest );
// class AdapterRealDataTest
AdapterRealDataTest::AdapterRealDataTest()
    : CppUnit::TestFixture()
{
}

AdapterRealDataTest::~AdapterRealDataTest()
{
}

void AdapterRealDataTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);
}


void AdapterRealDataTest::tearDown()
{
    delete _app;
}


void AdapterRealDataTest::test_deserialise_buffer()
{
    // Create a buffer to deserialise.
    typedef double real_t;
    std::vector<real_t> array(10, 0);
    size_t chunkSize = array.size() * sizeof(real_t);
    for (unsigned i = 0; i < array.size(); ++i) array[i] = i;
    QByteArray chunk = QByteArray::fromRawData((char*)&array[0], chunkSize);
    QBuffer buffer(&chunk);
    buffer.open(QBuffer::ReadOnly);

    // Create the data blob and the adapter.
    DoubleData data;
    ConfigNode config;
    AdapterRealData adapter(config);
    adapter.config(&data, chunkSize, QHash<QString, DataBlob*>());

    // Deserialise the data.
    CPPUNIT_ASSERT_NO_THROW(adapter.deserialise(&buffer));

    // Test values in the data blob.
    for (unsigned i = 0; i < array.size(); ++i)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(i, data.ptr()[i], 0.001);
}

} // namespace pelican

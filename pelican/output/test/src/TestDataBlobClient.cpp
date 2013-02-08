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

#include "TestDataBlobClient.h"
#include "output/Stream.h"
#include <QtCore/QCoreApplication>

namespace pelican {
namespace test {


/**
 * @details Constructs a TestDataBlobClient object.
 */
TestDataBlobClient::TestDataBlobClient()
{
}

/**
 * @details Destroys the TestDataBlobClient object.
 */
TestDataBlobClient::~TestDataBlobClient()
{
}

void TestDataBlobClient::send(const Stream& s)
{
    if( ! _streams.contains( s.name() ) )
    {
        _streams.insert( s.name() );
        emit newStreamsAvailable();
    }
    emit newData(s);
    // force any events to process
    QCoreApplication::processEvents( QEventLoop::WaitForMoreEvents, 200 );
}

QSet<QString> TestDataBlobClient::streams()
{
    return _streams;
}

void TestDataBlobClient::subscribe( const QSet<QString>& streams )
{
    _subs.unite(streams);
}

const QSet<QString>& TestDataBlobClient::subscriptions() const
{
    return _subs;
}

} // namespace test
} // namespace pelican

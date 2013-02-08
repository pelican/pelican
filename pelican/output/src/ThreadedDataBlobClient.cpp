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

#include "ThreadedDataBlobClient.h"
#include <QtCore/QSet>

#include "output/ThreadedClientImpl.h"
#include "output/DataBlobClient.h"

namespace pelican {

/**
 * @details Constructs a ThreadedDataBlobClient object.
 */
ThreadedDataBlobClient::ThreadedDataBlobClient(const ConfigNode& c)
    : AbstractDataBlobClient()
{
    _thread = new ThreadedClientImpl(c);
    _thread->start();
    while( ! _thread->client() ) { sleep(1); } // wait until the client exists
    connect( _thread->client(), SIGNAL(newData(const Stream&)),
                        this, SLOT( emitNewData(const Stream&) ), Qt::DirectConnection );
    connect( _thread->client(), SIGNAL(newStreamsAvailable()),
            this, SLOT( emitNewStreamsAvailable() ), Qt::DirectConnection );
}

/**
 * @details Destroys the ThreadedDataBlobClient object.
 */
ThreadedDataBlobClient::~ThreadedDataBlobClient()
{
    do { _thread->quit(); } while( ! _thread->wait(10) );
    delete _thread;
}

QSet<QString> ThreadedDataBlobClient::streams()
{
    if( _thread->client() )
        return _thread->client()->streams();
    QSet<QString> s;
    return s;
}

void ThreadedDataBlobClient::emitNewStreamsAvailable()
{
    emit newStreamsAvailable();
}

void ThreadedDataBlobClient::emitNewData(const Stream& s)
{
    emit newData(s);
}


/// set the host to listen to
void ThreadedDataBlobClient::setHost(const QString& host)
{
    _thread->client()->setHost(host);
}

/// set the port to listen on
void ThreadedDataBlobClient::setPort(quint16 port)
{
    _thread->client()->setPort(port);
}

void ThreadedDataBlobClient::subscribe( const QSet<QString>& streams )
{
    _thread->client()->subscribe(streams);
}

} // namespace pelican

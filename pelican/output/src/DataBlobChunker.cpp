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

#include "DataBlobChunker.h"
#include "DataBlobChunkerClient.h"
#include "PelicanClientProtocol.h"


namespace pelican {

/**
 *@details DataBlobChunker
 */
DataBlobChunker::DataBlobChunker(const ConfigNode& configNode)
    : AbstractChunker( configNode )
{
    _client = new DataBlobChunkerClient( new PelicanClientProtocol, this );
    setHost(configNode.getOption("connection", "host"));
    setPort(configNode.getOption("connection", "port").toUInt());

    QSet<QString> subs = QSet<QString>::fromList(configNode.getOptionList("subscribe","stream") );
    _client->subscribe( subs );
    foreach( const QString& s, subs ) {
        addChunkType(s);
        setDefaultAdapter( "DataBlobAdapter", s );
    }
}

/**
 *@details
 */
DataBlobChunker::~DataBlobChunker()
{
    delete _client;
}

void DataBlobChunker::setPort(quint16 port)
{
    _client->setPort( port );
}

quint16 DataBlobChunker::port() const
{
    return _client->port();
}

void DataBlobChunker::setHost(const QString& ipaddress)
{
    _client->setHost(ipaddress);
}

QIODevice* DataBlobChunker::newDevice() {
    return 0; // mark as self managed
}

} // namespace pelican

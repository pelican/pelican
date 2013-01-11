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

#include "DataBlobRelay.h"
#include "utility/Config.h"
#include "output/Stream.h"
#include "output/DataBlobClient.h"
#include "output/OutputStreamManager.h"
#include <iostream>


namespace pelican {

/**
 *@details DataBlobRelay
 */
DataBlobRelay::DataBlobRelay( const Config* config, const Config::TreeAddress& address )
{
    ConfigNode localConfig = config->get( address );
    // setup output manager
    Config::TreeAddress outputAddress = address;
    outputAddress << Config::NodeId("output","");
    _outputManager = new OutputStreamManager( config, outputAddress );

    // setup clients
    foreach( const ConfigNode& node, localConfig.getNodes("client")   ) {
        DataBlobClient* c = new DataBlobClient( node );
        _myClients.append( c );
        addClient( c );
    }
}

/**
 *@details
 */
DataBlobRelay::~DataBlobRelay()
{
    foreach( DataBlobClient* client, _myClients ) {
        delete client;
    }
    delete _outputManager;
}

void DataBlobRelay::connectToStream( AbstractOutputStream* streamer, const QString& stream) {
    _outputManager->connectToStream( streamer, stream );
}

void DataBlobRelay::addClient( AbstractDataBlobClient* client )
{
    _clients.append(client);
    connect( client, SIGNAL( newData(const Stream&) ),
             this, SLOT( _streamData( const Stream& ) ) );
}

void DataBlobRelay::_streamData( const Stream& s ) {
    boost::shared_ptr<DataBlob> data = s.data();
    _outputManager->send( data.get() , s.name() );
}

} // namespace pelican

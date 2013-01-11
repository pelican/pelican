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

#include "TestDataBlobServer.h"
#include "output/PelicanTCPBlobServer.h"
#include "data/DataBlob.h"
#include "utility/ConfigNode.h"
#include <QtCore/QCoreApplication>


namespace pelican {
namespace test {


/**
 *@details TestDataBlobServer
 */
TestDataBlobServer::TestDataBlobServer()
{
   QString xml( "<connection port=\"0\" />" );
   ConfigNode node( xml );
   _server = new PelicanTCPBlobServer( node );
}

/**
 *@details
 */
TestDataBlobServer::~TestDataBlobServer()
{
    delete _server;
}

QString TestDataBlobServer::host() const {
    return "127.0.0.1";
}

quint16 TestDataBlobServer::port() const {
    return _server->serverPort();
}

int TestDataBlobServer::clientsForStream( const QString& stream ) const {
    return _server->clientsForStream(stream);
}

void TestDataBlobServer::send(DataBlob* blob, const QString& stream ) {
    QString str=stream;
    if( str == "" ) { str=blob->type(); }
    _server->send( str, blob );
    QCoreApplication::processEvents();
}
} // namespace test
} // namespace pelican

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

#include "utility/test/SocketTester.h"

namespace pelican {
namespace test {

// class SocketTester
SocketTester::SocketTester()
{
    _init = false;
}

SocketTester::~SocketTester()
{
}

QTcpSocket& SocketTester::send(const QByteArray& block)
{
    init();
    if( _sock1.isOpen() ) {
        _sock1.disconnectFromHost();
    }
    Q_ASSERT( ! _server.hasPendingConnections() );
    _sock1.connectToHost(_server.serverAddress(),_server.serverPort());
    if( ! _sock1.waitForConnected(1000) ) {
        throw QString("connection timed out");
    }
    _sock1.write(block);
    while (_sock1.bytesToWrite() > 0)
        _sock1.waitForBytesWritten(-1);
    //_sock1.waitForBytesWritten(block.size());
    //_sock1.flush();
    _server.waitForNewConnection();

    Q_ASSERT( _server.hasPendingConnections() );
    QTcpSocket* sock = _server.nextPendingConnection();
    sock->waitForReadyRead();
    Q_ASSERT(sock->bytesAvailable() == block.size());
    return *sock;
}

void SocketTester::init()
{
    if( ! _init ) {
        _server.listen(QHostAddress::LocalHost, 0);
        _init = true;
    }
}

} // namespace test
} // namespace pelican


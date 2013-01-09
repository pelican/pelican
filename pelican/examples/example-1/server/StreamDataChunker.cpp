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

#include "StreamDataChunker.hpp"
#include <pelican/utility/Config.h>
#include <QtNetwork/QUdpSocket>

#include <iostream>
using namespace std;


StreamDataChunker::StreamDataChunker(const pelican::ConfigNode& config)
: AbstractChunker(config), chunkSize_(0), bytesRead_(0), chunkCounter_(0)
{
    // The host, port and data type are set in the base class.

    // Set the chunk size from the configuration.
    int numPackets = config.getOption("data", "packets", 0).toInt();
    qint64 packetSize = config.getOption("data", "packetSize", 0).toInt();
    chunkSize_ = numPackets * packetSize;
}

QIODevice* StreamDataChunker::newDevice()
{
    // Return an opened QUdpSocket.
    QUdpSocket* socket = new QUdpSocket;
    socket->bind(QHostAddress(host()), port());

    // Wait for the socket to bind.
    while (socket->state() != QUdpSocket::BoundState) {}

    return socket;
}

void StreamDataChunker::next(QIODevice* device)
{
    qDebug() << "StreamDataChunker::next() [" << chunkCounter_++ << "]";

    QUdpSocket* socket = static_cast<QUdpSocket*>(device);
    bytesRead_ = 0;

    // Get writable buffer space for chunk.
    pelican::WritableData chunk = getDataStorage(chunkSize_);
    if (chunk.isValid())
    {
        // Get pointer to start of chunk memory.
        char* ptr = (char*)chunk.ptr();

        // Read datagrams for chunk from the socket.
        while (isActive() && bytesRead_ < chunkSize_)
        {
            // Read the datagram, but avoid using pendingDatagramSize()
            if (!socket->hasPendingDatagrams())
            {
                // MUST wait for the next datagram
                socket->waitForReadyRead(100);
                continue;
            }
            qint64 maxlength = chunkSize_ - bytesRead_;
            qint64 length = socket->readDatagram(ptr + bytesRead_, maxlength);
            if (length > 0)
                bytesRead_ += length;
        }
    }

    // Discard the datagram if there is no space.
    else
    {
        socket->readDatagram(0, 0);
    }
}

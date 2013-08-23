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


#include "StreamChunker.hpp"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QIODevice>
#include <iostream>
#include <string>

using namespace std;

StreamChunker::StreamChunker(const pelican::ConfigNode& config)
: AbstractChunker(config), tcpServer_(0), chunkCounter_(0)
{
}

StreamChunker::~StreamChunker()
{
}

QIODevice* StreamChunker::newDevice()
{
    // * Note: The server had to be created here in the newDevice() function
    //   so that it is created in the DataReciever thread handling this chunker.
    // * As this function is also used to reconnect it needs to make sure the
    //   QTcpServer is not allocated and set up twice.
    if (tcpServer_ == 0) {
        tcpServer_ = new QTcpServer();
        cout << endl;
        cout << "StreamChunker: created a QTcpServer listening on:" << endl;
        cout << "StreamChunker:    host = " << host().toStdString() << endl;
        cout << "StreamChunker:    port = " << port() << endl;
        tcpServer_->listen(QHostAddress(host()), port());
    }
    cout << "StreamChunker: waiting for a connection!\n";
    tcpServer_->waitForNewConnection(-1);
    cout << "StreamChunker: connected!\n";
    timer_.start();
    return tcpServer_->nextPendingConnection();
}


void StreamChunker::next(QIODevice* device)
{
    // XXX next is slowed down significantly slowed down initially
    // by allocating the data chunks.
    // In benchmark mode make sure all chunks are allocated first before the
    // timing starts.

//    if (chunkCounter_ == 0)
//        timer_.start();

    QTcpSocket* socket = static_cast<QTcpSocket*>(device);
    quint64 totalBytesRead = 0;

    // Header values.
    quint32 packetSize = 0;
    quint32 packetId = 0;
    quint32 numPackets = 0;
    quint32 packetTime = 0;
    quint64 bytesRead = 0;
    quint32 reportInterval = 0;
    quint32 numHeaderFields = 5;
    size_t headerSize = numHeaderFields * sizeof(quint32);

//    while (socket->bytesAvailable() < 400016*100) {
//        socket->waitForReadyRead(-1);
//        cout << socket->bytesAvailable() << " / " <<400016*100 <<endl;
//    }

    // Read header values.
    while (isActive() && totalBytesRead != headerSize)
    {
        while (socket->bytesAvailable() < (int)(headerSize)) {
            socket->waitForReadyRead(-1);
        }
        bytesRead = socket->read((char*)&packetSize, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&packetId, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&packetTime, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&numPackets, (qint64)sizeof(qint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&reportInterval, (qint64)sizeof(qint32));
        totalBytesRead+=bytesRead;
    }

//    cout << chunkCounter_ << ", " << packetId << ", " << totalBytesRead <<endl;

    // Ask the data manager for a writable data chunk and get its data pointer.
    // XXX need some way to query the max chunk size and/or remaing space
    // in the data buffer here.
    pelican::WritableData chunk = getDataStorage(packetSize);
    // XXX should check chunk->isValid() before proceeding! (see also comment above)
    if (!chunk.isValid()) {
//        cerr << "StreamChunker::next(): Unable to get a valid chunk" << endl;
        throw QString("StreamChunker::next(): Unable to get a valid chunk");
    }
    char* chunkPtr = (char*)chunk.ptr();

    // Write the header into the chunk.
    quint32* header = reinterpret_cast<quint32*>(chunkPtr);
    header[0] = packetSize;
    header[1] = packetId;
    header[2] = numPackets;
    header[3] = packetTime;

    // XXX This may have an impact on performance.
    //socket->setReadBufferSize(1024*1024);

    quint64 dataRemaining = packetSize - headerSize;

    // XXX this size has an impact on performance.
    quint64 minReadSize = 128;

    // Read data block directly into the chunk.
    while (isActive() && dataRemaining > 0)
    {
        if (dataRemaining < minReadSize) minReadSize = dataRemaining;

        while (socket->bytesAvailable() < (qint64)minReadSize) {
            socket->waitForReadyRead(-1);
        }

        bytesRead = socket->read(chunkPtr+totalBytesRead, dataRemaining);
        totalBytesRead+=bytesRead;
        dataRemaining-=bytesRead;
    }

//    cout << chunkCounter_ << ", " << packetId << ", " << totalBytesRead <<endl;

    // XXX !!HORRIBLE BUG!!
    // XXX !!HORRIBLE BUG!!
    // XXX !!HORRIBLE BUG!!
    // bytes can be avail but next isn't called again....!
    // this is because next is called on the readyRead() signal!
    // --> this will be a problem any time the chunker can't keep up with the
    // input data rate??!!
    // XXX SOLUTION??
    // -- replace event loop in the DataReciever with a manual while loop
    //    which calls next if bytesAvailable() > 0
    // XXX http://qt-project.org/forums/viewthread/10184
//    cout << "bytes avail = " << socket->bytesAvailable() << endl << endl;
    chunkCounter_++;

    // Report performance if on the last packed sent from a run of the emulator.
    if (chunkCounter_%reportInterval == 0) {
        float elapsed = timer_.elapsed() / 1.0e3;
        float MiB = (packetSize * reportInterval) / (1024.0*1024.0);
        cout << endl;
        cout << string(80, '*') << endl;
        cout << __PRETTY_FUNCTION__ << endl;
        cout << std::string(80, '-') << endl;
        cout << "-- chunks complete = " << chunkCounter_ << endl;
        cout << "-- packet size     = " << packetSize << " bytes" << endl;
        cout << "-- packet size     = " << packetSize/(1024.0*1024.0) << " MiB" << endl;
        cout << "-- time taken      = " << elapsed << " seconds." << endl;
        cout << "-- MiB (sent)      = " << MiB << std::endl;
        cout << "-- MiB/s           = " << MiB/elapsed  << endl;
        cout << std::string(80, '*') << endl;
        timer_.restart();
    }
}



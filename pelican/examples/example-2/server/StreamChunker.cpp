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
#include <cstdio>
#include <string>

using namespace std;


/*
 * Creates chunks of StreamData from the packets send from the StreamEmulator.
 *
 *
 *
 *
 */

StreamChunker::StreamChunker(const pelican::ConfigNode& config)
: AbstractChunker(config), tcpServer_(0), chunkCounter_(0), reportCounter_(0),
  totalOverwriteCounter_(0), intervalOverwriteCounter_(0)
{
}

StreamChunker::~StreamChunker()
{
}

/*
 * Function that returns an IO device from which input data can be read.
 *
 * In order for the chunker (and therefore data server) to stay alive between
 * injection of data from invocations of the emulator binary used in this
 * example the chunker here creates a TCPServer and returns a connected
 * TCP socket whenever that becomes available upon connection with the emulator.
 *
 * The function blocks until a connection can be made.
 *
 * Each time the TCP socket disconnects, for example because the emulator has
 * finished, the DataReciever will attempt to reconnect by calling this method
 * again.
 *
 * TODO add a configurable timeout when waiting for new connections.
 */
QIODevice* StreamChunker::newDevice()
{
    // Note:
    // The server has to be created here (in the newDevice() function)
    // so that it is created in the DataReciever thread managing this chunker.
    // As this function is also used to reconnect, it needs to make sure the
    // QTcpServer is not allocated and set up twice.
    if (tcpServer_ == 0) {
        tcpServer_ = new QTcpServer();
        cout << endl;
        cout << "StreamChunker: Created a QTcpServer listening on:" << endl;
        cout << " * host = " << host().toStdString() << endl;
        cout << " * port = " << port() << endl;
        tcpServer_->listen(QHostAddress(host()), port());
    }
    cout << " * Waiting for a connection... " << flush;
    tcpServer_->waitForNewConnection(-1);
    cout << " Connected!" << endl << endl;
    timer_.start();
    return tcpServer_->nextPendingConnection();
}



/* Function that is called whenever there is data ready to be processed.
 * This function should block until all data for the chunk has been read from
 * the device.
 */
void StreamChunker::next(QIODevice* device)
{
    // Note:
    // The next() function is slowed down initially by allocating the
    // data chunks.
    // It is probably best therefore, when running the benchmark, to make sure
    // all chunks are allocated before the timing starts.

    QTcpSocket* socket = static_cast<QTcpSocket*>(device);
    quint64 totalBytesRead = 0;
    quint64 bytesRead = 0;

    // Header values.
    quint32 packetSize = 0;
    quint32 packetCounter = 0;
    quint32 packetTime = 0;
    qint32 numPackets = 0;
    qint32 reportInterval = 0;
    size_t headerSize = 3*sizeof(quint32) + 2*sizeof(qint32);

    // Read header values.
    while (isActive() && totalBytesRead != headerSize)
    {
        while (socket->bytesAvailable() < (qint64)(headerSize)) {
            socket->waitForReadyRead(-1);
        }
        bytesRead = socket->read((char*)&packetSize, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&packetCounter, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&packetTime, (qint64)sizeof(quint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&numPackets, (qint64)sizeof(qint32));
        totalBytesRead+=bytesRead;
        bytesRead = socket->read((char*)&reportInterval, (qint64)sizeof(qint32));
        totalBytesRead+=bytesRead;
    }

    //
    // Writable chunks are obtained using the getDataStorage() method
    // using the the following priority:
    //
    // 1) A Pre-allocated chunk in the buffer that has already been served
    //    and has been marked for reuse.
    // 2) Allocating a new chunk in the buffer if space allows.
    // 3) Overwriting the oldest chunk in the buffer that matches the space
    //    requirements.
    //
    // If none of these conditions can be met, an invalid chunk is returned.
    //

    // If there are no usable chunks, i.e. the buffer is full we will
    // be overwriting chunks.
#if 1
    if (numUsableChunks(packetSize) == 0) {
        ++totalOverwriteCounter_;
        ++intervalOverwriteCounter_;
    }
#endif

    // Ask the data manager for a writable data chunk and get its data pointer.
    pelican::WritableData chunk = getDataStorage(packetSize);

    // Check chunk->isValid() before proceeding!
    if (!chunk.isValid()) {
        QString error = "StreamChunker::next(): Unable to get a valid chunk. ";
        if (packetSize > maxChunkSize()) {
            error += QString(
                    "\n== The requested packet size is greater than the maximum"
                    "\n== chunk size allowed by the buffer.");
        }
        throw error;
    }

    // Extract the data pointer from the chunk
    char* chunkPtr = (char*)chunk.ptr();

    // Write the header into the chunk.
    quint32* header1 = reinterpret_cast<quint32*>(chunkPtr);
    header1[0] = packetSize;
    header1[1] = packetCounter;
    header1[2] = packetTime;
    qint32* header2 = reinterpret_cast<qint32*>(chunkPtr+3*sizeof(quint32));
    header2[0] = numPackets;
    header2[1] = reportInterval;

    // NOTE Setting the read buffer size may have an impact on performance.
#if 0
    socket->setReadBufferSize(2*1024*1024);
#endif

    // NOTE The minimum read size has an impact on performance and has not
    // been optimised.
    quint64 minReadSize = 1024;//2*1024*1024; // bytes

    // Read data block directly into the chunk.
    quint64 dataRemaining = packetSize - headerSize;
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

    chunkCounter_++;

    // Report performance
    if (chunkCounter_%reportInterval == 0)
    {
        const double B2MiB = 1.0/(1024.0*1024.0);
        int elapsed = timer_.elapsed();
        size_t dataReceived = packetSize * reportInterval;
        size_t maxBufferSize_ = maxBufferSize();
        size_t allocatedSize_ = allocatedSize();
        size_t usedSize_ = usedSize();
        size_t usableSize_ = usableSize(packetSize);
        // Buffer % full (how much of the buffer is in use as a %)
        double pBufferFull = ((maxBufferSize_-usableSize_)/(double)maxBufferSize_)*100.0;
        char prefix[10];
        int l = (reportCounter_ < 10) ? 2 : ((reportCounter_ < 100) ? 1 : 0);
        sprintf(prefix, "S[%s%llu] ", string(l, '-').c_str(), reportCounter_);
        printf("%s\n", string(80,'*').c_str());
        printf("%sTotal chunk count    = %llu\n", prefix, chunkCounter_);
        printf("%sChunk size           = %-7.3f MiB [%i B]\n", prefix,
                packetSize*B2MiB, packetSize);
        printf("%sTotal data received  = %.1f MiB\n", prefix,
                (quint64)packetSize*chunkCounter_*B2MiB);
        printf("%s\n", prefix);
        printf("%sBuffer state:\n", prefix);
        printf("%s* Percent full       = %.1f%%\n", prefix, pBufferFull);
        printf("%s* Total size         = %-7.3f MiB [%zu B]\n", prefix,
                maxBufferSize_*B2MiB, maxBufferSize_);
        printf("%s* Allocated size     = %-7.3f MiB [%zu B]\n", prefix,
                allocatedSize_*B2MiB, allocatedSize_);
        printf("%s* In use (active)    = %-7.3f MiB [%zu B]\n", prefix,
                usedSize_*B2MiB, usedSize_);
        printf("%s* Usable space       = %-7.3f MiB [%zu B]\n", prefix,
                usableSize_*B2MiB, usableSize_);
        printf("%s* Total chunks       = %i\n", prefix, numChunks());
        printf("%s* Active chunks      = %i\n", prefix, numActiveChunks());
        printf("%s* Expired chunks     = %i\n", prefix, numExpiredChunks());
        printf("%s* Usable chunks      = %i\n", prefix, numUsableChunks(packetSize));
        printf("%s* Overwritten chunks = %llu\n", prefix, totalOverwriteCounter_);
        printf("%s* Overwritten chunks = %i, %.1f%% (in report interval)\n", prefix,
                intervalOverwriteCounter_,
                (double)intervalOverwriteCounter_/reportInterval*100.0);
        printf("%s\n", prefix);
        printf("%sReport interval:\n", prefix);
        printf("%s* Chunks received    = %i\n", prefix, reportInterval);
        printf("%s* Data received      = %-7.3f MiB [%zu B] \n", prefix,
                dataReceived*B2MiB, dataReceived);
        printf("%s* Time taken         = %.3f s\n", prefix, elapsed*1.0e-3);
        printf("%s* Data rate          = ", prefix);
        if (elapsed > 0)
            printf("%-7.3f MiB/s\n", dataReceived*B2MiB/(elapsed*1.0e-3));
        else
            printf("---     MiB/s\n");
        printf("%s\n\n", string(80,'*').c_str());
        fflush(stdout);
        reportCounter_++;
        intervalOverwriteCounter_ = 0;
        timer_.restart();
    }
}


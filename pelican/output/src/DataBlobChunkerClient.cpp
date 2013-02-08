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

#include "DataBlobChunkerClient.h"
#include "DataBlobChunker.h"
#include "comms/DataBlobResponse.h"
#include "server/WritableData.h"
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDebug>
#include <iostream>


namespace pelican {


/**
 *@details DataBlobChunkerClient
 */
DataBlobChunkerClient::DataBlobChunkerClient( AbstractClientProtocol* protocol,
                                              DataBlobChunker* chunker )
    : AbstractDataBlobClient(0), _chunker(chunker)
{
    setProtocol( protocol );
}

/**
 *@details
 */
DataBlobChunkerClient::~DataBlobChunkerClient()
{
}

void DataBlobChunkerClient::dataReceived( DataBlobResponse* res ) {
    int hSize = res->serialisedSize();
    const QString& stream = res->dataName();
    WritableData writableData = _chunker->getDataStorage( hSize + res->dataSize(),
        stream );
    if( writableData.isValid() ) {
        // -- write out the Response info as a header
#if QT_VERSION >= 0x040700
        // more efficent - no memory allocations
        _byteArray.setRawData( (char*)(writableData.ptr()), hSize );
#else
        _byteArray = QByteArray::fromRawData( (char*)(writableData.ptr()), hSize);
#endif
        QDataStream dataStream( &_byteArray, QIODevice::WriteOnly );
        dataStream.setVersion(QDataStream::Qt_4_0);
        res->serialise( dataStream );
        while (_tcpSocket->bytesAvailable() < (qint64)res->dataSize())
            _tcpSocket -> waitForReadyRead(-1);
        // -- write out the actual DataBlob
        _tcpSocket->read( (char*)writableData.ptr() + hSize , res->dataSize() );
    }
    else {
        // discard the data
        while (_tcpSocket->bytesAvailable() < (qint64)res->dataSize())
            _tcpSocket -> waitForReadyRead(-1);
        _tcpSocket->read( res->dataSize() );
        std::cout << "DataBlobChunkerClient: discarding data for stream: "
                  << stream.toStdString() << std::endl;
    }
}

} // namespace pelican

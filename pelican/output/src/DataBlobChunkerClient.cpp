#include "DataBlobChunkerClient.h"
#include "DataBlobChunker.h"
#include "pelican/comms/DataBlobResponse.h"
#include "pelican/server/WritableData.h"
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

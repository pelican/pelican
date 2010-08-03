#include "DataBlobClient.h"
#include <QtNetwork/QTcpSocket>

#include "pelican/output/Stream.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataBlobFactory.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/comms/PelicanClientProtocol.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/comms/DataBlobResponse.h"
#include "pelican/comms/ServerResponse.h"
#include "pelican/comms/DataSupportRequest.h"
#include "pelican/comms/DataSupportResponse.h"

#include <iostream>
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 *@details DataBlobClient 
 */
DataBlobClient::DataBlobClient( const ConfigNode& configNode, QObject* parent )
    : QObject(parent)
{
    _protocol = new PelicanClientProtocol;
    _tcpSocket = new QTcpSocket;
    _blobFactory = new DataBlobFactory;

    setHost(configNode.getOption("connection", "host"));
    setPort(configNode.getOption("connection", "port").toUInt());
}

/**
 *@details
 */
DataBlobClient::~DataBlobClient()
{
    delete _tcpSocket;
    delete _protocol;
}

void DataBlobClient::setPort(quint16 port)
{
    _port = port;
}

quint16 DataBlobClient::port() const
{
    return _port;
}


void DataBlobClient::setHost(const QString& ipaddress)
{
    _server = ipaddress;
}

QSet<QString> DataBlobClient::streams()
{
    DataSupportRequest req;
    _sendRequest(&req);
    _streamInfo = false;
    while( ! _streamInfo ) {
        _response(); // run in this Thread
    }
    emit newStreamsAvailable();
    return _streams;
}

void DataBlobClient::subscribe(const QString& stream)
{
    QSet<QString> set;
    set.insert(stream);
    subscribe(set);
}

void DataBlobClient::subscribe(const QSet<QString>& streams)
{

    // Define the data type which the client will except and send request
    StreamDataRequest req;
    DataRequirements require;
    foreach( const QString& stream, streams )
    {
        if( ! _streamMap.contains(stream) )
            _streamMap.insert(stream, new Stream(stream) );
        require.setStreamData(stream);
    }
    req.addDataOption(require);
    _sendRequest(&req);

}

void DataBlobClient::_sendRequest( const ServerRequest* req ) const
{
    // construct request and connect to the port
    if( _tcpSocket->isOpen() )
        _tcpSocket->disconnectFromHost();

    QByteArray data = _protocol->serialise(*req);

    while (_tcpSocket->state() == QAbstractSocket::UnconnectedState) {
        _tcpSocket->connectToHost( _server, _port );

        if (!_tcpSocket -> waitForConnected(5000) || _tcpSocket -> state() == QAbstractSocket::UnconnectedState) {
            std::cerr << "Client could not connect to server:" << _server.toStdString() << " port:" << _port << std::endl;
            sleep(2);
            continue;
        }

        _tcpSocket->write(data);
        _tcpSocket->waitForBytesWritten(data.size());
        _tcpSocket->flush();
    }
}

void DataBlobClient::_response()
{
    _tcpSocket->waitForReadyRead();
    boost::shared_ptr<ServerResponse> r = _protocol->receive(*_tcpSocket);
    // Check what type of response we have
    switch( r -> type() ) {
        case ServerResponse::Error:  // Error occurred!!
            std::cerr << "DataBlobClient: Server Error: " << r -> message().toStdString() << std::endl;
            break;
        case ServerResponse::DataSupport:   // We have received supported data info
            {
                DataSupportResponse* res = static_cast<DataSupportResponse*>(r.get());
                _streams = res->streamData() + res->serviceData();
                _streamInfo = true;
            }
            break;
        case ServerResponse::Blob:   // We have received a blob
            {
                std::cout << "Blob Received" << std::endl;
                DataBlobResponse* res = static_cast<DataBlobResponse*>(r.get());
                while (_tcpSocket->bytesAvailable() < (qint64)res->dataSize())
                   _tcpSocket -> waitForReadyRead(-1);
                const QString& stream = res->dataName();

                // configure the appropriate Stream Object ready for sending
                Stream* s = _streamMap[stream];
                s->setData(_blob( res->blobClass(), res->dataName() ) );
                s->data()->deserialise(*_tcpSocket, res->byteOrder());

                emit newData(*s);
            }
            break;
        default:
            // all other repsonses are irrelevant
            std::cerr << "PelicanBlobClient: Unknown Response" << std::endl;
            break;
    }
}

DataBlob* DataBlobClient::_blob(const QString& type, const QString& /*stream*/)
{
    // TODO speedup with circular Blob buffers etc.
    return _blobFactory->create(type);
}

} // namespace pelican

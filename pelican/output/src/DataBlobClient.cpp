#include "DataBlobClient.h"
#include <QtNetwork/QTcpSocket>
#include <QtCore/QCoreApplication>

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

namespace pelican {

/**
 *@details DataBlobClient
 */
DataBlobClient::DataBlobClient( const ConfigNode& configNode, QObject* parent )
    : AbstractDataBlobClient(parent), _streamInfoSubscription(false)
{
    _destructor = false;
    _protocol = new PelicanClientProtocol;
    _tcpSocket = new QTcpSocket;
    _blobFactory = new DataBlobFactory;


    if( configNode.hasAttribute("verbose") )
        _verbose = 1;
    setHost(configNode.getOption("connection", "host"));
    setPort(configNode.getOption("connection", "port").toUInt());

    connect(_tcpSocket, SIGNAL( readyRead()),
                         this, SLOT( _response() ));
    connect(_tcpSocket, SIGNAL( disconnected()),
                         this, SLOT( _reconnect() ));

    // configured subsciptions
    QSet<QString> subs = QSet<QString>::fromList(configNode.getOptionList("subscribe","stream") );
    subscribe( subs );
}

/**
 *@details
 */
DataBlobClient::~DataBlobClient()
{
    _destructor = true;
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
    if( ! _streamInfoSubscription ) {
        _streamInfoSubscription = true;
        _streamInfo = false;
        if ( _requestStreamInfo() )
        {
            while ( (! _streamInfo) && _tcpSocket->state() == QAbstractSocket::ConnectedState ) {
                sleep(1);
                QCoreApplication::processEvents();
            }
        }
    }
    return _streams;
}

bool DataBlobClient::_requestStreamInfo()
{
    DataSupportRequest req;
    return _sendRequest(&req);
}

void DataBlobClient::subscribe(const QSet<QString>& streams)
{

    // Define the data type which the client will except and send request
    if( streams.size() > 0 ) {
        StreamDataRequest req;
        DataRequirements require;
        foreach( const QString& stream, streams )
        {
            verbose(QString("Subscribing to stream : \"") + stream + " \"" );
            if( ! _streamMap.contains(stream) )
                _streamMap.insert(stream, new Stream(stream) );
            require.setStreamData(stream);
        }
        if( require != _currentSubscription )
        {
            req.addDataOption(require);
            _sendRequest(&req);
            _subscriptions.unite(streams);
        }
    }

}

bool DataBlobClient::_sendRequest( const ServerRequest* req )
{
    // construct request and connect to the port
    QByteArray data = _protocol->serialise(*req);

    bool sent = _connect() ;
    if ( sent )
    {
        _tcpSocket->write(data);
        _tcpSocket->waitForBytesWritten(data.size());
        _tcpSocket->flush();
    }
    return sent;
}

void DataBlobClient::_reconnect()
{
    if( ! _destructor ) {
        verbose( "DataBlobClient: Connection lost - reconnecting()", 1);
        _currentSubscription.clear();
        subscribe( _subscriptions );
        if( _streamInfoSubscription )
            _requestStreamInfo();
    }
}

bool DataBlobClient::_connect()
{
    while (_tcpSocket->state() == QAbstractSocket::UnconnectedState) {
       _tcpSocket->connectToHost( _server, _port );
       if (!_tcpSocket -> waitForConnected(5000) || _tcpSocket -> state() == QAbstractSocket::UnconnectedState) {
           std::cerr << "Client could not connect to server:" << _server.toStdString() << " port:" << _port << std::endl;
           sleep(2);
           continue;
       }
    }
    return true;
}

void DataBlobClient::_response()
{
    boost::shared_ptr<ServerResponse> r = _protocol->receive(*_tcpSocket);
    // Check what type of response we have
    switch( r -> type() ) {
        case ServerResponse::Error:  // Error occurred!!
            std::cerr << "DataBlobClient: Server Error: " << r -> message().toStdString() << std::endl;
            break;
        case ServerResponse::DataSupport:   // We have received supported data info
            {
                verbose("DataSupport information received");
                DataSupportResponse* res = static_cast<DataSupportResponse*>(r.get());
#ifdef BROKEN_QT_SET_HEADER
                _streams = res->streamData();
                _streams.unite(res->serviceData());
#else
                _streams = res->streamData() + res->serviceData();
#endif
                emit newStreamsAvailable();
                _streamInfo = true;
            }
            break;
        case ServerResponse::Blob:   // We have received a blob
            {
                verbose("DataBlob received");
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
    // TODO speedup with circular Blob buffers etc?
    return _blobFactory->create(type);
}

} // namespace pelican

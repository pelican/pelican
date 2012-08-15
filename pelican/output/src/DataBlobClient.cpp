#include "DataBlobClient.h"
#include <QtNetwork/QTcpSocket>
#include <QtCore/QCoreApplication>

#include "pelican/output/Stream.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataBlobFactory.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/comms/PelicanClientProtocol.h"
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
    setProtocol( new PelicanClientProtocol );
    _blobFactory = new DataBlobFactory;


    if( configNode.hasAttribute("verbose") )
        _verbose = 1;
    setHost(configNode.getOption("connection", "host"));
    setPort(configNode.getOption("connection", "port").toUInt());


    // configured subsciptions
    QSet<QString> subs = QSet<QString>::fromList(configNode.getOptionList("subscribe","stream") );
    subscribe( subs );
}

/**
 *@details
 */
DataBlobClient::~DataBlobClient()
{
}

QSet<QString> DataBlobClient::streams()
{
    if( ! _streamInfoSubscription ) {
        _streamInfoSubscription = true;
        _streamInfo = false;
        if ( requestStreamInfo() )
        {
            while ( (! _streamInfo) && _tcpSocket->state() == QAbstractSocket::ConnectedState ) {
                sleep(1);
                QCoreApplication::processEvents();
            }
        }
    }
    return AbstractDataBlobClient::streams();
}


void DataBlobClient::onReconnect()
{
    if( _streamInfoSubscription )
        requestStreamInfo();
}

void DataBlobClient::onSubscribe(const QString& stream)
{
    if( ! _streamMap.contains(stream) )
        _streamMap.insert(stream, new Stream(stream) );
}

void DataBlobClient::dataSupport( DataSupportResponse* res ) {
    AbstractDataBlobClient::dataSupport( res );
    emit newStreamsAvailable();
    _streamInfo = true;
}

void DataBlobClient::dataReceived( DataBlobResponse* res ) {
    while (_tcpSocket->bytesAvailable() < (qint64)res->dataSize())
        _tcpSocket -> waitForReadyRead(-1);
    const QString& stream = res->dataName();

    // configure the appropriate Stream Object ready for sending
    Stream* s = _streamMap[stream];
    s->setData(_blob( res->blobClass(), res->dataName() ) );
    s->data()->deserialise(*_tcpSocket, res->byteOrder());

    emit newData(*s);
}

boost::shared_ptr<DataBlob> DataBlobClient::_blob(const QString& type, const QString& /*stream*/)
{
    // TODO speedup with circular Blob buffers etc?
    return boost::shared_ptr<DataBlob>( _blobFactory->create(type) );
}

} // namespace pelican

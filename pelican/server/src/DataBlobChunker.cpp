#include "DataBlobChunker.h"


namespace pelican {

/**
 *@details DataBlobChunker 
 */
DataBlobChunker::DataBlobChunker(const ConfigNode& config)
    : AbstractChunker( config )
{
    _protocol = new PelicanClientProtocol;
    setHost(configNode.getOption("connection", "host"));
    setPort(configNode.getOption("connection", "port").toUInt());

    QSet<QString> subs = QSet<QString>::fromList(configNode.getOptionList("subscribe","stream") );
    subscribe( subs );
}

/**
 *@details
 */
DataBlobChunker::~DataBlobChunker()
{
    delete _protocol;
}

void DataBlobChunker::setPort(quint16 port)
{
    _port = port;
}

quint16 DataBlobChunker::port() const
{
    return _port;
}

void DataBlobChunker::setHost(const QString& ipaddress)
{
    _server = ipaddress;
}

QIODevice* DataBlobChunker::newDevice() {
    QTcpSocket* s = new QTcpSocket;
    s->connectToHost( _server, _port );
    // subscribe to the streams
    StreamDataRequest req;
    return s;
}

void DataBlobChunker::next(QIODevice*) {
    boost::shared_ptr<ServerResponse> r = _protocol->receive(*_tcpSocket);
    // ignore any other messages than data sends
    switch( r -> type() ) {
        case ServerResponse::Blob:
            {
                while (_tcpSocket->bytesAvailable() < (qint64)res->dataSize())
                   _tcpSocket->waitForReadyRead(-1);
                const QString& stream = res->dataName();
            }
            break;
        default:
            break;
    }
}

} // namespace pelican

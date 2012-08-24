#include "DataBlobChunker.h"
#include "DataBlobChunkerClient.h"
#include "PelicanClientProtocol.h"


namespace pelican {

/**
 *@details DataBlobChunker 
 */
DataBlobChunker::DataBlobChunker(const ConfigNode& configNode)
    : AbstractChunker( configNode )
{
    _client = new DataBlobChunkerClient( new PelicanClientProtocol, this );
    setHost(configNode.getOption("connection", "host"));
    setPort(configNode.getOption("connection", "port").toUInt());

    QSet<QString> subs = QSet<QString>::fromList(configNode.getOptionList("subscribe","stream") );
    _client->subscribe( subs );
    foreach( const QString& s, subs ) {
        addChunkType(s);
    }
}

/**
 *@details
 */
DataBlobChunker::~DataBlobChunker()
{
    delete _client;
}

void DataBlobChunker::setPort(quint16 port)
{
    _client->setPort( port );
}

quint16 DataBlobChunker::port() const
{
    return _client->port();
}

void DataBlobChunker::setHost(const QString& ipaddress)
{
    _client->setHost(ipaddress);
}

QIODevice* DataBlobChunker::newDevice() {
    return 0; // mark as self managed
}

} // namespace pelican

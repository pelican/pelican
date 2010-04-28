#include <QBuffer>
#include "PelicanTCPBlobServer.h"


#include "pelican/server/WritableData.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details PelicanTCPBlobServer
 */
PelicanTCPBlobServer::PelicanTCPBlobServer(QObject* parent)
    : AbstractBlobServer(), QThread(parent)
{
    //_dataManger = new DataManager(config); TODO
    start();
}

/**
 *@details
 */
PelicanTCPBlobServer::~PelicanTCPBlobServer()
{
}

void PelicanTCPBlobServer::send(const QString& type, const QByteArray& incoming)
{
    StreamDataBuffer* buf = _dataManager->getStreamBuffer(type);
    WritableData data = buf->getWritable(incoming.size());
    if( data.isValid() ) {
        data.write(&incoming, incoming.size() );
    }
}

void PelicanTCPBlobServer::run()
{
    // TODO
    /*
    while( _run ) {
        foreach(const QString& type, clientsRequestingData.keys() ) {
                LockedData d =_dataManger->getNext(type) ) {
                if( d.isValid() ) {
                    // send it out to all clients for this dat type
                    foreach(connectedClient, clientsRequestingData[type] )
                    {
                    }
                }
            }
        }
    }
    */
}

} // namespace pelican

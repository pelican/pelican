#include <QBuffer>
#include "AbstractBlobServer.h"

#include "pelican/data/DataBlob.h"
#include "pelican/server/WritableData.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details AbstractBlobServer 
 */
AbstractBlobServer::AbstractBlobServer()
{
}

/**
 *@details
 */
AbstractBlobServer::~AbstractBlobServer()
{
}

/**
 *@details
 */
void AbstractBlobServer::send(const QString& streamName, const QByteArray& incoming)
{
    StreamDataBuffer* buf = _dataManager->getStreamBuffer(streamName);
    WritableData data = buf -> getWritable(incoming.size());
    if( data.isValid() ) {
        data.write(&incoming, incoming.size() );
    }
}

/**
 *@details
 */
void AbstractBlobServer::queue(const QString& stream, DataBlob* blob)
{
    const QByteArray& data = blob->serialise();
    send(stream,data);
}

} // namespace pelican

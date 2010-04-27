#include <QBuffer>
#include "AbstractBlobServer.h"


#include "pelican/data/DataBlob.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details AbstractBlobServer 
 */
AbstractBlobServer::AbstractBlobServer(QObject* parent  )
    : QObject(parent)
{
}

/**
 *@details
 */
AbstractBlobServer::~AbstractBlobServer()
{
}

void AbstractBlobServer::queue(const QString& stream, DataBlob* blob)
{
    const QBuffer& data = blob->serialise();
    send(stream,data);
}

} // namespace pelican

#include "AbstractChunker.h"
#include "DataManager.h"
#include "WritableData.h"

#include "utility/memCheck.h"

namespace pelican {


// class AbstractChunker 
AbstractChunker::AbstractChunker(DataManager* dm)
    : _dm(dm)
{
}

AbstractChunker::~AbstractChunker()
{
}

/* Methods to interact with the server */
WritableData AbstractChunker::getDataStorage(const QString& type, size_t size) const
{
    return _dm->getWritableData(type, size);
}

/* Default imlementations */
QAbstractSocket* AbstractChunker::newSocket() const
{
    return new QUdpSocket;
}

} // namespace pelican

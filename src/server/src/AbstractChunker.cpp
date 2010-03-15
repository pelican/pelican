#include "AbstractChunker.h"
#include "DataManager.h"
#include "WritableData.h"

#include "utility/memCheck.h"

namespace pelican {


// class AbstractChunker 
AbstractChunker::AbstractChunker(const QString& type, DataManager* dm)
    : _dm(dm), _type(type)
{
}

AbstractChunker::~AbstractChunker()
{
}

const QString& AbstractChunker::type() const
{
    return _type;
}

/* Methods to interact with the server */
WritableData AbstractChunker::getDataStorage(size_t size) const
{
    return _dm->getWritableData(_type, size);
}

/* Default imlementations */
QAbstractSocket* AbstractChunker::newSocket() const
{
    return new QUdpSocket;
}

} // namespace pelican

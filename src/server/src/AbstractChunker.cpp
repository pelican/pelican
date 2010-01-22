#include "AbstractChunker.h"


#include "utility/memCheck.h"

namespace pelican {


// class AbstractChunker 
AbstractChunker::AbstractChunker()
{
}

AbstractChunker::~AbstractChunker()
{
}

QAbstractSocket* AbstractChunker::newSocket() const
{
    return new QUdpSocket;
}

} // namespace pelican

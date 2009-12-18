#include "StreamDataBuffer.h"
#include "StreamData.h"
#include "LockedData.h"
#include <QMutexLocker>


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataBuffer 
StreamDataBuffer::StreamDataBuffer()
    : DataBuffer()
{
}

StreamDataBuffer::~StreamDataBuffer()
{
}

LockedData StreamDataBuffer::getNext()
{
//    QMutexLocker locker(&_mutex);
    return LockedData(_serveQueue.dequeue());
}

} // namespace pelican

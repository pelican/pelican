#include "StreamDataBuffer.h"
#include "StreamData.h"
#include "LockedData.h"
#include "WritableData.h"
#include <QMutexLocker>


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataBuffer 
StreamDataBuffer::StreamDataBuffer(QObject* parent)
    : DataBuffer(parent)
{
    _max = 10000; //TODO make configurable
    _maxChunkSize = _max;
    _space = _max; 
}

StreamDataBuffer::~StreamDataBuffer()
{
    foreach( StreamData* data, _data)
    {
        delete data;
    }
}

LockedData StreamDataBuffer::getNext()
{
    QMutexLocker locker(&_mutex);
    if( ! _serveQueue.isEmpty() )
        return LockedData(_serveQueue.dequeue());
       // std::cout << "HERE" << std::endl;
    return LockedData(0);
}

WritableData StreamDataBuffer::getWritable(size_t size)
{
    QMutexLocker locker(&_mutex);
    if( ! _emptyQueue.isEmpty() )
    {
        // iterate through until we find a container big enough
        QQueue<StreamData*> temp = _emptyQueue;
        do {
            StreamData* d = temp.dequeue();
            if( sizeof(*d) >= size ) {
                // we found one - so our work is done
                return WritableData( d );
            }
        }
        while( ! temp.isEmpty() );
    }
    // There are no empty containers already available so we
    // create a new data Object if we have enough space
    if(size <= _space && size <= _maxChunkSize )
    {
        void* d = malloc(size);
        if( d ) {
            _space -= size;
            StreamData* s = new StreamData(d, size);
            _data.append(s);
            Q_ASSERT(connect( s, SIGNAL(unlocked(Data*)), this, SLOT(activateData(Data*) ) ));
            return WritableData( s );
        }
    }
    return WritableData(0); // no free containers so we return an invalid
}

void StreamDataBuffer::activateData(Data* data)
{
    activateData(static_cast<StreamData*>(data) );
}

void StreamDataBuffer::activateData(StreamData* data)
{
    QMutexLocker locker(&_mutex);
    if( data->isValid() )
    {
        _serveQueue.enqueue(data);
    }
    else
    {
        _emptyQueue.enqueue(data);
    }
}

} // namespace pelican

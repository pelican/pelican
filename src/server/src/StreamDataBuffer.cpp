#include "DataManager.h"
#include "StreamDataBuffer.h"
#include "StreamData.h"
#include "LockedData.h"
#include "WritableData.h"
#include <QMutexLocker>
#include <stdlib.h>


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataBuffer 
StreamDataBuffer::StreamDataBuffer(DataManager* manager, QObject* parent)
    : DataBuffer(parent),_manager(manager)
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

void StreamDataBuffer::setDataManager(DataManager* manager)
{
    _manager = manager;
}

void StreamDataBuffer::getNext(LockedData& ld)
{
    QMutexLocker locker(&_mutex);
    if( ! _serveQueue.isEmpty() )
        ld.setData(_serveQueue.dequeue());
    ld.setData(0);
}

WritableData StreamDataBuffer::getWritable(size_t size)
{
    QMutexLocker locker(&_mutex);
    StreamData* d = _getWritable(size);

    // prepare the object for use
    // - add Service Data info
    if( d ) {
        d->reset();
        _manager->associateServiceData(d);
    }
    return WritableData( d );
}

StreamData* StreamDataBuffer::_getWritable(size_t size)
{
    if( ! _emptyQueue.isEmpty() )
    {
        // iterate through until we find a container big enough
        QQueue<StreamData*> temp = _emptyQueue;
        do {
            StreamData* d = temp.dequeue();
            if( sizeof(*d) >= size ) {
                // we found one - so our work is done
                return d;
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
            Q_ASSERT(connect( s, SIGNAL(unlockedWrite()), this, SLOT(activateData() ) ));
            return s;
        }
    }
    return 0; // no free containers so we return an invalid
}

void StreamDataBuffer::activateData()
{
    activateData(static_cast<StreamData*>( sender() ) );
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

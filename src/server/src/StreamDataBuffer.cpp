#include "DataManager.h"
#include "StreamDataBuffer.h"
#include "LockableStreamData.h"
#include "LockedData.h"
#include "WritableData.h"
#include <QMutexLocker>
#include <stdlib.h>


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataBuffer 
StreamDataBuffer::StreamDataBuffer(const QString& type, DataManager* manager, QObject* parent)
    : DataBuffer(type, parent),_manager(manager)
{
    _max = 10000; //TODO make configurable
    _maxChunkSize = _max;
    _space = _max; 
}

StreamDataBuffer::~StreamDataBuffer()
{
    foreach( LockableStreamData* data, _data)
    {
        delete data;
    }
}

void StreamDataBuffer::setDataManager(DataManager* manager)
{
    _manager = manager;
}

void StreamDataBuffer::getNext(LockedData& lockedData)
{
    QMutexLocker locker(&_mutex);
    if( ! _serveQueue.isEmpty() )
        lockedData.setData(_serveQueue.dequeue());
    lockedData.setData(0);
}

WritableData StreamDataBuffer::getWritable(size_t size)
{
    QMutexLocker locker(&_mutex);
    LockableStreamData* d = _getWritable(size);

    // prepare the object for use
    // - add Service Data info
    if( d ) {
        d->reset();
        _manager->associateServiceData(d);
    }
    return WritableData( d );
}

/**
 * @details
 * Private method to get a writable block of memory of the given size.
 *
 * @return Returns a pointer to the LockableStreamData object to use.
 *
 * @param[in] size The size in bytes of the requested block of writable memory.
 */
LockableStreamData* StreamDataBuffer::_getWritable(size_t size)
{
    if( ! _emptyQueue.isEmpty() )
    {
        // Make a copy of the empty queue.
        QQueue<LockableStreamData*> temp = _emptyQueue;

        // Iterate through until we find a container big enough.
        do {
            LockableStreamData* lockableData = temp.dequeue();
            if( lockableData->size() >= size ) {
                // We found one - so our work is done.
                return lockableData;
            }
        }
        while( ! temp.isEmpty() );
    }

    // There are no empty containers already available, so we
    // create a new data object if we have enough space.
    if(size <= _space && size <= _maxChunkSize )
    {
        void* memory = malloc(size);
        if( memory ) {
            _space -= size;
            LockableStreamData* lockableData = new LockableStreamData(_type, memory, size);
            _data.append(lockableData);
            Q_ASSERT(connect( lockableData, SIGNAL(unlockedWrite()), this, SLOT(activateData() ) ));
            return lockableData;
        }
    }

    // No free containers and no space left, so we return an invalid pointer.
    return 0;
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlockedWrite() signal.
 */
void StreamDataBuffer::activateData()
{
    activateData( static_cast<LockableStreamData*>(sender()) );
}

void StreamDataBuffer::activateData(LockableStreamData* data)
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

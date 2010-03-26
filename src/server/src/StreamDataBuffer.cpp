#include "DataManager.h"
#include "StreamDataBuffer.h"
#include "LockableStreamData.h"
#include "LockedData.h"
#include "WritableData.h"
#include "comms/StreamData.h"
#include <QMutexLocker>
#include <stdlib.h>
#include <iostream>


#include "utility/memCheck.h"

namespace pelican {


// class StreamDataBuffer
StreamDataBuffer::StreamDataBuffer(const QString& type, DataManager* manager, QObject* parent)
    : DataBuffer(type, parent),_manager(manager)
{
    // These are in bytes:
    _max = 10000; //TODO make configurable
    _maxChunkSize = _max;
    _space = _max;
}

StreamDataBuffer::~StreamDataBuffer()
{
    foreach( LockableStreamData* data, _data)
        delete data;
}

/**
 * @details
 * TODO please!
 */
void StreamDataBuffer::getNext(LockedData& lockedData)
{
    QMutexLocker locker(&_mutex);
    std::cout << "StreamDataBuffer::getNext() . Serve queue size " << _serveQueue.size() << std::endl;

    for (int i = 0; i < _serveQueue.size(); ++i) {
        LockableStreamData* lockable = _serveQueue[i];
        void *ptr = lockable->data()->operator *();
        std::cout << "StreamDataBuffer::getNext(): Queue pos: " << i << " ptr = " << ptr << std::endl;
        std::cout << "StreamDataBuffer::getNext(): Queue pos: " << i << " Content: " << reinterpret_cast<double*>(ptr)[0] << std::endl;
    }

    if( ! _serveQueue.isEmpty() ) {
        std::cout << "StreamDataBuffer::getNext(): Serve queue not empty."  << std::endl;
        lockedData.setData(_serveQueue.dequeue());
        return;
    } else {
        std::cout << "StreamDataBuffer::getNext(): Serve queue empty."  << std::endl;
    }
    lockedData.setData(0);
}

/**
 * @details
 * Gets a writable data object of the given size and returns it.
 *
 * @param[in] size The size of the writable data to return.
 */
WritableData StreamDataBuffer::getWritable(size_t size)
{
    // XXX remove
    std::cout << std::endl;
    QMutexLocker locker(&_mutex);
    LockableStreamData* d = _getWritable(size);

    // prepare the object for use
    // - add Service Data info
    if( d ) {
        d->reset();
        if (!_manager)
            throw QString("StreamDataBuffer::getWritable(): No data manager.");
        _manager->associateServiceData(d);
    }
    std::cout << "StreamDataBuffer: Returning WritableData object" << std::endl;
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
      std::cout << "StreamDataBuffer::_getWritable(): Returning pre-allocated data from the emptyQueue." << std::endl;
        // Make a copy of the empty queue.
//        QQueue<LockableStreamData*> temp = _emptyQueue;

      // Iterate through until we find a container big enough.
      int i = 0;
      do {
          LockableStreamData* lockableData = _emptyQueue[i];

          if( lockableData->maxSize() >= size ) {
              // We found one, so our work is done.
              _emptyQueue.removeAt(i);
              return lockableData;
          }
          ++i;
      }
      while ( i < _emptyQueue.size() );

//       do {
//            LockableStreamData* lockableData = temp.dequeue();
//            if( lockableData->maxSize() >= size ) {
//                // We found one, so our work is done.
//
//                return lockableData;
//            }
//        }
//        while( ! temp.isEmpty() );
    }

    // There are no empty containers already available, so we
    // create a new data object if we have enough space.
    if(size <= _space && size <= _maxChunkSize )
    {
        void* memory = malloc(size);
        std::cout << "StreamDataBuffer::_getWritable(): Allocated memory." << std::endl;
        if (memory) {
            _space -= size;
            LockableStreamData* lockableData = new LockableStreamData(_type, memory, size);
            _data.append(lockableData);
            std::cout << "Appended the data to the list" << std::endl;
            Q_ASSERT(connect( lockableData, SIGNAL(unlockedWrite()), this, SLOT(activateData() ) ));
            Q_ASSERT(connect( lockableData, SIGNAL(unlocked()), this, SLOT(deactivateData() ) ));
            return lockableData;
        }
    }

    // No free containers and no space left, so we return an invalid pointer.
    return 0;
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlockedWrite() signal. It calls the method to activate the data
 * chunk that emitted the signal by putting it onto the serve queue.
 */
void StreamDataBuffer::activateData()
{
    std::cout << "StreamDataBuffer: Activate data slot" << std::endl;
    activateData( static_cast<LockableStreamData*>(sender()) );
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlocked() signal. It calls the method to deactivate the data
 * chunk that emitted the signal by putting it onto the empty queue.
 */
void StreamDataBuffer::deactivateData()
{
    deactivateData(static_cast<LockableStreamData*>( sender() ) );
}

/**
 * @details
 * Puts the specified chunk of data on the empty queue, which allows its
 * space to be reused.
 *
 * TODO Must implement a reset method on data() to de-associate the service data.
 */
void StreamDataBuffer::deactivateData(LockableStreamData* data)
{
    std::cout << "StreamDataBuffer: Deactivating data" << std::endl;
    QMutexLocker locker(&_mutex);
    data->reset();
//    _emptyQueue.enqueue(data);
    _emptyQueue.push_back(data);
}

/**
 * @details
 * Puts the specified chunk of data on the queue ready to be served.
 */
void StreamDataBuffer::activateData(LockableStreamData* data)
{
    QMutexLocker locker(&_mutex);
    if( data->isValid() ) {
        std::cout << "StreamDataBuffer: Activate data: is valid" << std::endl;
        _serveQueue.enqueue(data);
    } else {
        std::cout << "StreamDataBuffer: Activate data not valid" << std::endl;
//        _emptyQueue.enqueue(data);
        _emptyQueue.push_back(data);
    }
}

} // namespace pelican

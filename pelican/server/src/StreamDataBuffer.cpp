#include "pelican/server/StreamDataBuffer.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/WritableData.h"
#include "pelican/comms/StreamData.h"

#include <QtCore/QMutexLocker>
#include <stdlib.h>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the stream data buffer.
 *
 * @param type         A string containing the type of data held in the buffer.
 * @param manager      A pointer to the data manager to use.
 * @param max          The maximum size of the buffer in bytes.
 * @param maxChunkSize The maximum chunk size in bytes.
 * @param parent       (Optional.) Pointer to the object's parent.
 */
StreamDataBuffer::StreamDataBuffer(const QString& type, DataManager* manager,
        const size_t max, const size_t maxChunkSize, QObject* parent) :
        DataBuffer(type, parent),_manager(manager)
{
    _max = max;
    _maxChunkSize = maxChunkSize;
    _space = _max; // initially empty so the space = max size.
}


/**
 * @details
 * Destroys the stream data buffer, freeing the memory held by it.
 */
StreamDataBuffer::~StreamDataBuffer()
{
    foreach (LockableStreamData* data, _data) {
        free(data->data()->data());
        delete data;
    }
}


/**
 * @details
 * Gets the next block of data to serve.
 */
void StreamDataBuffer::getNext(LockedData& lockedData)
{
    QMutexLocker locker(&_mutex);

    // Check if the serve queue is empty.
    if (_serveQueue.isEmpty()) {
        //std::cout << "StreamDataBuffer::getNext(): Queue empty." << std::endl;
        lockedData.setData(0); // Return an invalid data block.
        return;
    }

    // XXX This loop is used for printing only - remove it.
//    for (int i = 0; i < _serveQueue.size(); ++i) {
//        LockableStreamData* lockable = _serveQueue[i];
//        void *ptr = lockable->data()->data();
//        std::cout << "StreamDataBuffer::getNext(): Queue position: " << i <<
//                " Content: " << *reinterpret_cast<double*>(ptr) << std::endl;
//    }

    // Remove the data from the serve queue.
    lockedData.setData(_serveQueue.dequeue());
}


/**
 * @details
 * Gets a writable data object of the given size and returns it.
 *
 * @param[in] size The size of the writable data to return.
 */
WritableData StreamDataBuffer::getWritable(size_t size)
{
    QMutexLocker locker(&_writeMutex);
    LockableStreamData* lockableStreamData = _getWritable(size);

    // Prepare the object for use by adding Service Data info.
    if (lockableStreamData) {
        lockableStreamData->reset();
        if (!_manager)
            throw QString("StreamDataBuffer::getWritable(): No data manager.");
        _manager->associateServiceData(lockableStreamData);
    }

    //std::cout << "StreamDataBuffer: Returning WritableData object" << std::endl;
    return WritableData(lockableStreamData);
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
//    std::cout << "StreamDataBuffer::_getWritable() Empty queue size: " <<
//            _emptyQueue.size() << std::endl;

    // Return a pre-allocated block from the empty queue, if one exists.
    for (int i = 0; i < _emptyQueue.size(); ++i) {
        LockableStreamData* lockableData = _emptyQueue[i];

        if( lockableData->maxSize() >= size ) {
            // We found one, so our work is done.
            _emptyQueue.removeAt(i);
            return lockableData;
        }
    }

    // There are no empty containers already available, so we
    // create a new data object if we have enough space.
    if (size <= _space && size <= _maxChunkSize)
    {
        void* memory = calloc(size, sizeof(char)); // Released in destructor.
        if (memory) {
            _space -= size;
            LockableStreamData* lockableData = new LockableStreamData(_type, memory, size);
            _data.append(lockableData); // Add to the list of known data.
            connect(lockableData, SIGNAL(unlockedWrite()), SLOT(activateData()));
            connect(lockableData, SIGNAL(unlocked()), SLOT(deactivateData()));
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
    activateData(static_cast<LockableStreamData*>(sender()));
}


/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlocked() signal. It calls the method to deactivate the data
 * chunk that emitted the signal by putting it onto the empty queue.
 */
void StreamDataBuffer::deactivateData()
{
    deactivateData(static_cast<LockableStreamData*>(sender()));
}


/**
 * @details
 * Puts the specified chunk of data on the empty queue, which allows the
 * space it occupied to be reused.
 */
void StreamDataBuffer::deactivateData(LockableStreamData* data)
{
    //std::cout << "StreamDataBuffer::deactivateData()" << std::endl;

    QMutexLocker locker(&_mutex);
    if (!data->served()) {
        _serveQueue.prepend(data);
        return;
    }
    //std::cout << "StreamDataBuffer: Deactivating data" << std::endl;
    data->reset();

    QMutexLocker writeLocker(&_writeMutex);
    _emptyQueue.push_back(data);
}


/**
 * @details
 * Puts the specified chunk of data on the queue ready to be served.
 */
void StreamDataBuffer::activateData(LockableStreamData* data)
{
    if (data->isValid()) {
        QMutexLocker locker(&_mutex);
        //std::cout << "StreamDataBuffer::activateData(): Valid" << std::endl;
        _serveQueue.enqueue(data);
    } else {
        QMutexLocker writeLocker(&_writeMutex);
        //std::cout << "StreamDataBuffer::activateData(): Not valid" << std::endl;
        _emptyQueue.push_back(data);
    }
}

} // namespace pelican

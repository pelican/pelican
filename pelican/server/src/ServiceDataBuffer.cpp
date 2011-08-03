#include "pelican/server/ServiceDataBuffer.h"

#include "pelican/server/LockedData.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/LockableServiceData.h"

#include <QtCore/QMutexLocker>
#include <stdlib.h>

namespace pelican {

/**
 * @details
 * Constructs the service data buffer.
 *
 * @param type         A string containing the type of data held in the buffer.
 * @param max          The maximum size of the buffer in bytes.
 * @param maxChunkSize The maximum chunk size in bytes.
 * @param parent       (Optional.) Pointer to the object's parent.
 */
ServiceDataBuffer::ServiceDataBuffer(const QString& type,
        const size_t max, const size_t maxChunkSize, QObject* parent) :
        AbstractDataBuffer(type, parent)
{
    _max = max;
    _maxChunkSize = maxChunkSize;
    if( _maxChunkSize == 0 ) {
        _maxChunkSize = max;
    }
    _space = _max; // Buffer initially empty so space = max size.
    _newData = 0;
    _id = 0;
}

/**
 * @details
 * Destroys the service data buffer.
 */
ServiceDataBuffer::~ServiceDataBuffer()
{
    delete _newData;
    foreach (LockableServiceData* data, _data) {
        free(data->data()->data());
        delete data;
    }
}

/**
 * @details
 * Returns the specified version of the service data in the passed LockedData
 * object. If this data is unavailable then return an invalid.
 *
 * @param[in,out] lockedData  A reference to the LockedData object to set.
 * @param[in] version         The version identifier of the data to return.
 */
void ServiceDataBuffer::getData(LockedData& lockedData, const QString& version)
{
    QMutexLocker lock(&_mutex);
    lockedData.setData(_data.value(version));
}

/**
 * @details
 * Returns the current version of the service data in the passed LockedData
 * object.
 *
 * @param[in,out] lockedData  A reference to the LockedData object to set.
 */
void ServiceDataBuffer::getCurrent(LockedData& lockedData)
{
    QMutexLocker lock(&_mutex);
    lockedData.setData(_data.value(_current));
}

/**
 * @details
 * Gets a writable data object of the given size and returns it.
 * This object is locked, and on unlocking will become the current service data.
 *
 * @param[in] size The size of the writable data to return.
 */
WritableData ServiceDataBuffer::getWritable(size_t size)
{
    if (! _newData) {
        QMutexLocker lock(&_mutex);
        for (int i = 0; i < _expiredData.size(); ++i) {
            LockableServiceData* lockableData = _expiredData[i];
            if (lockableData->maxSize() >= size) {
                // We found one, so reuse it and remove it from the
                // expired data queue.
                _data.remove(lockableData->id());
                _expiredData.removeAt(i);
                lockableData->setSize(size);
                return lockableData;
            }
        }

        // Create a new data object if we have enough space.
        if (size <= _space && size <= _maxChunkSize)
        {
            void* memory = calloc(size, sizeof(char)); // Released in destructor.
            if (memory) {
                _space -= size;
                _newData = new LockableServiceData(_type, memory, size);
                connect(_newData, SIGNAL(unlockedWrite()), SLOT(activateData()));
                connect(_newData, SIGNAL(unlocked()), SLOT(deactivateData()));
                return WritableData(_newData);
            }
        }
    }
    return WritableData(0); // no free containers so we return an invalid
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlockedWrite() signal. It calls the method to activate the data
 * chunk that emitted the signal.
 */
void ServiceDataBuffer::activateData()
{
    activateData(static_cast<LockableServiceData*>(sender()));
}

/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlocked() signal. It calls the method to deactivate the data
 * chunk that emitted the signal.
 */
void ServiceDataBuffer::deactivateData()
{
    deactivateData(static_cast<LockableServiceData*>(sender()));
}

/**
 * @details
 * Deactivates the passed LockableServiceData object, and checks if it can
 * be expired.
 *
 * The data cannot be deleted as soon as there is no longer any stream data
 * referencing it, since a client may still ask for the data.
 *
 * If the passed data object is not the "current" service data (which is
 * associated with any incoming stream data), it is marked as available for
 * removal in the event of the buffer becoming full.
 *
 * @param[in] data Pointer to the LockableServiceData object to activate.
 */
void ServiceDataBuffer::deactivateData(LockableServiceData* data)
{
   if (data->id() != _current && ! _expiredData.contains(data))
        _expiredData.append(data);
}

/**
 * @details
 * Activates the passed LockableServiceData object, making it available
 * to be served.
 *
 * @param[in] data Pointer to the LockableServiceData object to activate.
 */
void ServiceDataBuffer::activateData(LockableServiceData* data)
{
    if (data->isValid())
    {
        QMutexLocker lock(&_mutex);

        // Determine the version ID for this object.
        QString id = data->id();
        if (id.isEmpty()) {
            // Generate a unique ID if not already set.
            id.setNum(++_id);
            data->setId(id);
        }

        // Move from pending into the active data hash.
        _data.insert(id, data);
        _newData = 0;

        // Set the current pointer to this one.
        _current = id;
    }
}

} // namespace pelican

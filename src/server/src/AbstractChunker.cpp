#include "AbstractChunker.h"

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Destroys the AbstractChunker, deleting the IO device it uses.
 */
AbstractChunker::~AbstractChunker()
{
    // FIXME Why don't we delete the socket or device here?
//    if (_device)
//        delete _device;
}

/**
 * @details
 * Returns a writable data object of the specified /p size.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
 *
 * 
 *
 * @param[in] size  The size of the chunk requested on the buffer.
 */
WritableData AbstractChunker::getDataStorage(size_t size) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");
    return _dataManager->getWritableData(_type, size);
}

} // namespace pelican

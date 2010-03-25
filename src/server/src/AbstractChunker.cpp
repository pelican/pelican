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
 *
 */
WritableData AbstractChunker::getDataStorage(size_t size) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");
    return _dataManager->getWritableData(_type, size);
}

} // namespace pelican

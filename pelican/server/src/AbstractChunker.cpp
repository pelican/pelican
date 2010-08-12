#include "pelican/server/AbstractChunker.h"
#include "pelican/utility/ConfigNode.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 *  Setup from the configuration file
 *  option 1:
 *    listen to an incomming port
 *    <connection host="dataHost" port="12345" />
 */

AbstractChunker::AbstractChunker(const ConfigNode& config)
{
    // Initialise members.
    _dataManager = NULL;
    _device = NULL;
    _type = config.getOption("data", "type", "");
    _host = config.getOption("connection", "host", "");
    _port = config.getOption("connection", "port", "0").toUInt();
    _active = true;
}

/**
 * @details
 * Destroys the AbstractChunker.
 */
AbstractChunker::~AbstractChunker()
{
    stop();
    if (_device)
        delete _device;
}

/**
 * @details
 * Returns a writable data object of the specified \p size.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
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

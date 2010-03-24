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

} // namespace pelican

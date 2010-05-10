#include "AbstractClientProtocol.h"


#include "pelican/data/DataBlob.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details AbstractClientProtocol 
 */
AbstractClientProtocol::AbstractClientProtocol()
{
}

/**
 *@details
 */
AbstractClientProtocol::~AbstractClientProtocol()
{
}

/**
 * @details
 * provide a convenience interface for finding the appropriate DataBlob object
 * to fill.
 */
DataBlob* AbstractClientProtocol::_getDataBlob(const QString& type)
{
    return _blobFactory.create(type);
}

} // namespace pelican

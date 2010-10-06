#include "AbstractDataBlobClient.h"

#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details Constructs a AbstractDataBlobClient object.
 */
AbstractDataBlobClient::AbstractDataBlobClient(QObject* parent)
    : QObject(parent)
{
}

/**
 * @details Destroys the AbstractDataBlobClient object.
 */
AbstractDataBlobClient::~AbstractDataBlobClient()
{
}

} // namespace pelican

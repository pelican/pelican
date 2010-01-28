#include "core/AbstractDataClient.h"
#include "data/DataRequirements.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 */
AbstractDataClient::AbstractDataClient()
{
}

/**
 * @details
 * Destroys the data client.
 */
AbstractDataClient::~AbstractDataClient()
{
}

/**
 * @details
 * Sets the IO device.
 */
void AbstractDataClient::setDevice(QIODevice *device)
{
    _inputDataStream.setDevice(device);
}

} // namespace pelican

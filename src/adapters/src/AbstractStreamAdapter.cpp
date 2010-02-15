#include "adapters/AbstractStreamAdapter.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Creates a new abstract Pelican adapter with the given configuration.
 */
AbstractStreamAdapter::AbstractStreamAdapter(const ConfigNode& config)
: AbstractAdapter(config)
{
}


/**
 * @details
 * Destroys the adapter.
 */
AbstractStreamAdapter::~AbstractStreamAdapter()
{
}


/**
 * @details
 */
const AbstractStreamAdapter& AbstractStreamAdapter::config(DataBlob* data,
        std::size_t size, QHash<QString, DataBlob*>& serviceData)
{
    _data = data;
    _size = size;
    _serviceData = serviceData;
    return *this;
}

} // namespace pelican


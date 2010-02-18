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
    _serviceData = QHash<QString, DataBlob*>();
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
AbstractStreamAdapter& AbstractStreamAdapter::config(DataBlob* data,
        std::size_t size, QHash<QString, DataBlob*> serviceData)
{
    _data = data;
    _chunkSize = size;
    _serviceData = serviceData;
    return *this;
}



} // namespace pelican


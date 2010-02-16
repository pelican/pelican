#include "adapters/AbstractServiceAdapter.h"
#include "adapters/AbstractAdapter.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Creates a new abstract Pelican adapter with the given configuration.
 */
AbstractServiceAdapter::AbstractServiceAdapter(const ConfigNode& config)
: AbstractAdapter(config)
{
}


/**
 * @details
 * Destroys the adapter.
 */
AbstractServiceAdapter::~AbstractServiceAdapter()
{
}


/**
 * @details
 */
AbstractServiceAdapter& AbstractServiceAdapter::config(DataBlob* data,
        std::size_t size)
{
    _data = data;
    _chunkSize = size;
    return *this;
}

} // namespace pelican


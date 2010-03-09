#include "adapters/AbstractAdapter.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Creates a new abstract Pelican adapter with the given configuration.
 */
AbstractAdapter::AbstractAdapter(AbstractAdapter::AdapterType_t type, const ConfigNode& /*config*/)
    : _type(type)
{
    _chunkSize = 0;
    _data = NULL;
}


/**
 * @details
 * Destroys the adapter.
 */
AbstractAdapter::~AbstractAdapter()
{
}

AbstractAdapter::AdapterType_t AbstractAdapter::type() const
{
    return _type;
}


} // namespace pelican


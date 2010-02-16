#include "adapters/AbstractAdapter.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Creates a new abstract Pelican adapter with the given configuration.
 */
AbstractAdapter::AbstractAdapter(const ConfigNode& config)
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


QDataStream& operator>> (QDataStream& in, AbstractAdapter& adapter)
{
    adapter.deserialise(in);
    return in;
}

} // namespace pelican


#include "adapters/AbstractAdapter.h"
#include "utility/ConfigNode.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Creates a new abstract Pelican adapter with the given configuration.
 */
AbstractAdapter::AbstractAdapter(const ConfigNode& config)
{
}


/**
 * @details
 * Destroys the adapter.
 */
AbstractAdapter::~AbstractAdapter()
{
}


inline QDataStream& operator>> (QDataStream& in, const AbstractAdapter& adapter)
{
    adapter.deseralise(in);
    return in;
}

} // namespace pelican


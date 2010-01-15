#include "modules/ZenithModelVisibilities.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithModelVisibilities::ZenithModelVisibilities(const QDomElement& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
ZenithModelVisibilities::~ZenithModelVisibilities()
{
}

} // namespace pelican

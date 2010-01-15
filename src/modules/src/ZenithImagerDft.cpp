#include "modules/ZenithImagerDft.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ZenithImagerDft::ZenithImagerDft(const QDomElement& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
ZenithImagerDft::~ZenithImagerDft()
{
}

} // namespace pelican

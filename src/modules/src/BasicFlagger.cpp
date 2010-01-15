#include "modules/BasicFlagger.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
BasicFlagger::BasicFlagger(const QDomElement& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
BasicFlagger::~BasicFlagger()
{
}

} // namespace pelican

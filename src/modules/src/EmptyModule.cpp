#include "modules/EmptyModule.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
EmptyModule::EmptyModule(const QDomElement& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
EmptyModule::~EmptyModule()
{
}

} // namespace pelican

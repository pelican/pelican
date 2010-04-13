#include "pelican/modules/EmptyModule.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
EmptyModule::EmptyModule(const ConfigNode& config)
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

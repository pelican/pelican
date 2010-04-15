#include "pelican/modules/EmptyModule.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_MODULE(EmptyModule)

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

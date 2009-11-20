#include "ModuleFactory.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * ModuleFactory constructor.
 */
ModuleFactory::ModuleFactory(Config *config)
{
    _config = config;
}

/**
 * @details
 * ModuleFactory destructor.
 */
ModuleFactory::~ModuleFactory()
{
}

/**
 * @details
 * Public interface to create a new module.
 *
 * @return
 * Returns a pointer to the newly-created module.
 */
AbstractModule* ModuleFactory::createModule(const QString& name)
{
}

/**
 * @details
 * Creates a new module.
 */
void ModuleFactory::_createModule(const QString& name, const Config& config)
{
}

} // namespace pelican

#include "modules/TestModule.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Creates a new test Pelican module with the given configuration.
 */
TestModule::TestModule(const QDomElement& config)
    : AbstractModule(config)
{
    addStreamData("TestStream");
    addServiceData("TestService");
}

/**
 * @details
 * Destroys the test Pelican module.
 */
TestModule::~TestModule()
{
}

} // namespace pelican

#include "modules/EmptyModule.h"

#include "utility/memCheck.h"

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

void EmptyModule::setDataRequirements(const DataRequirements& req)
{
    addStreamData(req.streamData());
    addServiceData(req.serviceData());
}


} // namespace pelican

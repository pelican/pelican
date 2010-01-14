#include "core/AbstractPipeline.h"
#include "core/ModuleFactory.h"
#include "core/PipelineApplication.h"
#include "modules/AbstractModule.h"
#include <QtGlobal>
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * AbstractPipeline constructor.
 */
AbstractPipeline::AbstractPipeline()
{
    /* Store pointer to module factory */
    _moduleFactory = PipelineApplication::moduleFactory();
    if (_moduleFactory == NULL)
        throw QString("Module factory doesn't exist: Create a PipelineApplication first.");

    /* Call init() to create the pipeline modules */
    init();
}

/**
 * @details
 * AbstractPipeline destructor.
 */
AbstractPipeline::~AbstractPipeline()
{
}

/**
 * @details
 * Creates a new module using the application's module factory.
 */
AbstractModule* AbstractPipeline::createModule(const QString& moduleName)
{
    AbstractModule* module = _moduleFactory->createModule(moduleName);
    _modules.append(module);
    return module;
}

/**
 * @details
 * Computes and returns the data required by the modules in this pipeline.
 */
const DataRequirements& AbstractPipeline::dataRequired()
{
    /* Clear existing data requirements */
    _data.clear();

    /* Loop over all the modules in the pipeline */
    foreach (AbstractModule* module, _modules) {
        _data += module->requiredData();
    }
    return _data;
}

} // namespace pelican

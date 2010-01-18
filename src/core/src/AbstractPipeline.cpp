#include "core/AbstractModule.h"
#include "core/AbstractPipeline.h"
#include "core/ModuleFactory.h"
#include "core/PipelineApplication.h"
#include "core/PipelineDriver.h"
#include <QtGlobal>
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * AbstractPipeline constructor.
 */
AbstractPipeline::AbstractPipeline()
{
    /* Initialise members */
    _moduleFactory = NULL;
    _pipelineDriver = NULL;
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
    /* Check that the module factory exists */
    if (_moduleFactory == NULL)
           throw QString("Module factory doesn't exist.");

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
    /* Loop over all the modules in the pipeline */
    foreach (AbstractModule* module, _modules) {
        _data += module->requiredData();
    }
    return _data;
}

/**
 * @details
 * Sets the pointer to the module factory.
 *
 * @param[in] moduleFactory Pointer to the module factory to use.
 */
void AbstractPipeline::setModuleFactory(ModuleFactory* moduleFactory)
{
    _moduleFactory = moduleFactory;
}

/**
 * @details
 * Sets the pointer to the pipeline driver.
 *
 * @param[in] pipelineDriver Pointer to the pipeline driver to use.
 */
void AbstractPipeline::setPipelineDriver(PipelineDriver* pipelineDriver)
{
    _pipelineDriver = pipelineDriver;
}

/**
 * @details
 * Stops the pipeline driver.
 */
void AbstractPipeline::stop()
{
    if (_pipelineDriver == NULL)
        throw QString("");

    _pipelineDriver->stop();
}

} // namespace pelican

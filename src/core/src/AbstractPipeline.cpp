#include "modules/AbstractModule.h"
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
 * AbstractPipeline destructor. This is implemented as a virtual so that
 * subclasses of AbstractPipeline will be destroyed cleanly.
 */
AbstractPipeline::~AbstractPipeline()
{
}

/**
 * @details
 * Creates a new module using the pipeline application's module factory.
 * The known module types are listed in the ModuleFactory class description.
 *
 * This function should be called from the init() method to create the modules
 * required by the pipeline.
 *
 * @param[in] type The type-name of the module to create.
 *                 See the list of known module types in ModuleFactory.
 *
 * @return The method returns a pointer to the newly-created module.
 */
AbstractModule* AbstractPipeline::createModule(const QString& type)
{
    /* Check that the module factory exists */
    if (_moduleFactory == NULL)
           throw QString("Module factory doesn't exist.");

    AbstractModule* module = _moduleFactory->createModule(type, name());
    _modules.append(module);
    return module;
}

/**
 * @details
 * Computes and returns the data required by the modules in this pipeline.
 *
 * @return Returns a const-reference to the DataRequirements object held
 *         by the pipeline.
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
 * @param[in] factory Pointer to the module factory to use.
 */
void AbstractPipeline::setModuleFactory(ModuleFactory* factory)
{
    _moduleFactory = factory;
}

/**
 * @details
 * Sets the name of the pipeline to \p name.
 * This should be called from inside init().
 *
 * @param[in] name The name of the pipeline.
 */
void AbstractPipeline::setName(const QString& name)
{
    _pipelineName = name;
}

/**
 * @details
 * Sets the pointer to the pipeline driver.
 *
 * @param[in] driver Pointer to the pipeline driver to use.
 */
void AbstractPipeline::setPipelineDriver(PipelineDriver* driver)
{
    _pipelineDriver = driver;
}

/**
 * @details
 * This protected function is provided for the pipeline to stop the
 * pipeline driver, and should only be called under abnormal termination
 * conditions.
 */
void AbstractPipeline::stop()
{
    if (_pipelineDriver == NULL)
        throw QString("No pipeline driver.");

    _pipelineDriver->stop();
}

} // namespace pelican

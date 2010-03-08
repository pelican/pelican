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
 * @param[in] name The name of the module to create. Name is used in the XML
 *                 configuration to differentiate the use of several modules
 *                 of the same type within a pipeline.
 *
 *
 * @return The method returns a pointer to the newly-created module.
 */
AbstractModule* AbstractPipeline::createModule(const QString& type,
        const QString& name)
{
    /* Check that the module factory exists */
    if (_moduleFactory == NULL)
           throw QString("AbstractPipeline: Module factory doesn't exist.");

    AbstractModule* module = _moduleFactory->createModule(type, name);
    _modules.append(module);
    return module;
}

/**
 * @details
 * Computes and returns all the data required by the modules in this pipeline.
 *
 * @return Returns all the data required by all modules in the pipeline.
 */
const DataRequirements AbstractPipeline::requiredDataAll()
{
    DataRequirements reqDataAll;

    /* Loop over all the modules in the pipeline */
    foreach (AbstractModule* module, _modules) {
        reqDataAll += module->requiredDataAll();
    }
    return reqDataAll;
}

/**
 * @details
 * Computes and returns all the generated data required by the modules in
 * this pipeline.
 *
 * @return Returns the generated data required by all modules in the pipeline.
 */
const DataRequirements AbstractPipeline::requiredDataGenerated()
{
    DataRequirements reqDataGenerated;

    /* Loop over all the modules in the pipeline */
    foreach (AbstractModule* module, _modules) {
        reqDataGenerated += module->requiredDataGenerated();
    }
    return reqDataGenerated;
}

/**
 * @details
 * Computes and returns all the remote data required by the modules in this
 * pipeline.
 *
 * @return Returns the remote data required by all modules in the pipeline.
 */
const DataRequirements AbstractPipeline::requiredDataRemote()
{
    DataRequirements reqDataRemote;

    /* Loop over all the modules in the pipeline */
    foreach (AbstractModule* module, _modules) {
        reqDataRemote += module->requiredDataRemote();
    }
    return reqDataRemote;
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

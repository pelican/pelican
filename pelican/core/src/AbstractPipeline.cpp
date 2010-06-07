#include "pelican/modules/AbstractModule.h"
#include "pelican/core/AbstractPipeline.h"
#include "pelican/core/PipelineApplication.h"
#include "pelican/core/PipelineDriver.h"
#include <QtCore/QtGlobal>
#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * AbstractPipeline constructor.
 */
AbstractPipeline::AbstractPipeline()
{
    // Initialise members.
    _blobFactory = NULL;
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
 * Creates a new data blob using the pipeline application's data blob factory.
 *
 * This function should be called from the init() method to create the data
 * blobs local to the pipeline.
 *
 * @param[in] type The type-name of the data blob to create.
 *
 * @return The method returns a pointer to the newly-created data blob.
 */
DataBlob* AbstractPipeline::createBlob(const QString& type)
{
    // Check that the blob factory exists.
    if (_blobFactory == NULL)
           throw QString("AbstractPipeline::createBlob(): No data blob factory.");

    DataBlob* blob = _blobFactory->create(type);
    return blob;
}

/**
 * @details
 * Creates a new module using the pipeline application's module factory.
 *
 * This function should be called from the init() method to create the modules
 * required by the pipeline.
 *
 * @param[in] type The type-name of the module to create.
 * @param[in] name The name of the module to create. Name is used in the XML
 *                 configuration to differentiate the use of several modules
 *                 of the same type within a pipeline.
 *
 * @return The method returns a pointer to the newly-created module.
 */
AbstractModule* AbstractPipeline::createModule(const QString& type,
        const QString& name)
{
    // Check that the module factory exists.
    if (_moduleFactory == NULL)
           throw QString("AbstractPipeline::createModule(): No module factory.");

    AbstractModule* module = _moduleFactory->create(type, name);
    _modules.append(module);
    return module;
}

/**
 * @details
 * Requests remote data from the data client.
 */
void AbstractPipeline::requestRemoteData(QString type)
{
    _requiredDataRemote.addStreamData(type);
}

/**
 * @details
 * Returns all the remote data required by this pipeline.
 *
 * @return Returns the remote data required by the pipeline.
 */
const DataRequirements& AbstractPipeline::requiredDataRemote() const
{
    return _requiredDataRemote;
}

/**
 * @details
 * Sets the pointer to the data blob factory.
 *
 * @param[in] factory Pointer to the data blob factory to use.
 */
void AbstractPipeline::setBlobFactory(FactoryGeneric<DataBlob>* factory)
{
    _blobFactory = factory;
}

/**
 * @details
 * Sets the pointer to the module factory.
 *
 * @param[in] factory Pointer to the module factory to use.
 */
void AbstractPipeline::setModuleFactory(FactoryConfig<AbstractModule>* factory)
{
    _moduleFactory = factory;
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
        throw QString("AbstractPipeline::stop(): No pipeline driver.");

    _pipelineDriver->stop();
}

} // namespace pelican

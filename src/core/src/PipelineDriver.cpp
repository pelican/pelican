#include <QString>
#include <QtGlobal>
#include <QtDebug>
#include "DataClientFactory.h"
#include "core/AbstractDataClient.h"
#include "core/FileDataClient.h"
#include "core/AbstractPipeline.h"
#include "core/ModuleFactory.h"
#include "core/PipelineDriver.h"
#include "data/DataBlob.h"
#include "data/DataBlobFactory.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"
#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * PipelineDriver constructor, which takes pointers to the allocated factories.
 */
PipelineDriver::PipelineDriver(DataBlobFactory* blobFactory,
        ModuleFactory* moduleFactory, DataClientFactory* clientFactory
){
    // Initialise member variables.
    _run = false;
    _dataClient = NULL;

    // Store pointers to factories.
    _blobFactory = blobFactory;
    _moduleFactory = moduleFactory;
    _clientFactory = clientFactory;
    Q_ASSERT(_blobFactory != 0 );
    Q_ASSERT(_moduleFactory != 0 );
    Q_ASSERT(_clientFactory != 0 );
}

/**
 * @details
 * PipelineDriver destructor. This deletes all the registered pipelines
 * and the data blobs.
 */
PipelineDriver::~PipelineDriver()
{
    /* Delete the pipelines */
    foreach (AbstractPipeline* pipeline, _registeredPipelines) {
        delete pipeline;
    }
    _registeredPipelines.clear();

    /* Delete the data blobs */
    foreach (DataBlob* dataBlob, _dataHash) {
        delete dataBlob;
    }
    _dataHash.clear();

}

/**
 * @details
 * Public interface to register the given \p pipeline with the driver.
 * The pipeline is initialised when this method is called.
 *
 * Registered pipelines will be deleted when the class is destroyed.
 *
 * @param[in] pipeline Pointer to the allocated pipeline.
 */
void PipelineDriver::registerPipeline(AbstractPipeline *pipeline)
{
    // Store the pointer to the registered pipeline.
    _registeredPipelines.append(pipeline);

    // Set up and initialise the pipeline.
    pipeline->setModuleFactory(_moduleFactory);
    pipeline->setPipelineDriver(this);
    pipeline->init();

    // Store the remote data requirements.
    _allDataRequirements.append(pipeline->requiredDataRemote());
}

/**
 * @details
 * Sets the given data client based on the named argument.
 * The client is only created after the pipelines have been initialised,
 * when start() is called.
 *
 * The recognised values are:
 * - FileDataClient
 *
 * @param[in] name The type of the data client to create.
 */
void PipelineDriver::setDataClient(QString name)
{
    _dataClientName = name;
}

/**
 * @details
 * Iterates over all registered pipelines to determine the required data and
 * starts the data flow through the pipelines.
 *
 * This public method is called by PipelineApplication start().
 */
void PipelineDriver::start()
{
    // Check for at least one registered pipeline.
    if (_registeredPipelines.isEmpty())
        throw QString("PipelineDriver::start(): No pipelines.");

    // Store the remote data requirements for each pipeline.
    foreach (AbstractPipeline* pipeline, _registeredPipelines) {
        _determineDataRequirements(pipeline);
        _pipelines.insert(pipeline->requiredDataRemote(), pipeline);
    }

    // Create the data blobs.
    _createDataBlobs();

    // Create the data client.
    if (!_dataClientName.isEmpty())
        _createDataClient(_dataClientName);

    // Enter main program loop.
    _run = true;
    while (_run) {
        // Get the data from the client.
        QHash<QString, DataBlob*> validData;
        if (_dataClient)
            validData = _dataClient->getData(_dataHash);

        // Run all the pipelines compatible with this data hash.
        bool ranPipeline = false;
        QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
        while (i != _pipelines.end()) {
            if (i.key().isCompatible(validData)) {
                ranPipeline = true;
                i.value()->run(_dataHash);
            }
            ++i;
        }

        // Check if no pipelines were run.
        if (!ranPipeline)
            throw QString("PipelineDriver::start(): No pipelines run.");
    }
}

/**
 * @details
 * Stops the pipeline driver.
 * This method should be called by a running pipeline.
 */
void PipelineDriver::stop()
{
    _run = false;
}

/**
 * @details
 * This method allocates all the required data blobs and inserts pointers to
 * them into the data hash.
 *
 * This private method is called by start().
 */
void PipelineDriver::_createDataBlobs()
{
    // Data required by all pipelines.
    QSet<QString> allData;
    foreach (DataRequirements req, _allDataRequirements) {
        allData += req.serviceData() + req.streamData();
    }

    /* Iterate over data requirements to create blobs if they don't exist */
    foreach (QString type, allData) {
        if ( ! _dataHash.contains(type) )
            _dataHash.insert(type, _blobFactory->create(type));
    }
}

/**
 * @details
 * Creates the data client after a call to the public method setDataClient()
 * sets the client type.
 *
 * Throws an exception of type QString if the data client is unknown.
 *
 * @param[in] type The type of the data client to create.
 */
void PipelineDriver::_createDataClient(QString type)
{
    _dataClient = _clientFactory->create(type, dataRequirements() );
}


/**
 * @details
 * Returns the list of remote data required for each pipeline.
 *
 * @return
 */
const QList<DataRequirements>& PipelineDriver::dataRequirements() const
{
    return _allDataRequirements;
}

/**
 * @details
 * Private method to find the data requirements of the given pipeline.
 * This is called by _initialisePipelines().
 */
void PipelineDriver::_determineDataRequirements(AbstractPipeline* pipeline)
{
    /* Check that the set of stream data required for this pipeline does not
     * intersect the set of stream data required by another.
     * Data is not currently copied, so this ensures that two pipelines do not
     * try to modify the same data. */
    QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
    while (i != _pipelines.end()) {
        if ((i.key().streamData() & pipeline->requiredDataRemote().streamData()).size() > 0) {
            throw QString("Multiple pipelines requiring the same remote stream data are not supported.");
        }
        ++i;
    }
}

} // namespace pelican

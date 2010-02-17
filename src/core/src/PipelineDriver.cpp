#include <QString>
#include <QtGlobal>
#include <QtDebug>
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
PipelineDriver::PipelineDriver (
        AdapterFactory* adapterFactory,
        DataBlobFactory* blobFactory,
        ModuleFactory* moduleFactory
){
    /* Initialise member variables */
    _run = false;
    _config = NULL;
    _dataClient = NULL;
    _moduleFactory = moduleFactory;
    _blobFactory = blobFactory;
    _adapterFactory = adapterFactory;
}

/**
 * @details
 * PipelineDriver destructor. This deletes all the registered pipelines and
 * the data blobs.
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
 * Public interface to register the pipeline with the driver.
 * Registered pipelines will be deleted when the class is destroyed.
 */
void PipelineDriver::registerPipeline(AbstractPipeline *pipeline)
{
    _registeredPipelines.append(pipeline);
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
 * @param[in] config The application's configuration object.
 */
void PipelineDriver::setDataClient(QString name, Config* config)
{
    _dataClientName = name;
    _config = config;
}

/**
 * @details
 * Iterates over all registered pipelines to determine the required data and
 * starts the data flow through the pipelines.
 */
void PipelineDriver::start()
{
    /* Initialise the pipelines and determine their data requirements */
    _initialisePipelines();

    /* Create the data blobs */
    _createDataBlobs(_reqDataAll);

    /* Create the data client */
    _createDataClient(_dataClientName, _config);

    /* Enter main program loop */
    _run = true;
    while (_run) {
        /* Get the data from the client */
        QHash<QString, DataBlob*> validData = _dataClient->getData(_dataHash);

        /* Check for empty data */
        if (validData.isEmpty())
            throw QString("No data returned from client.");

        /* Run all the pipelines compatible with this data hash */
        QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
        while (i != _pipelines.end()) {
            if (i.key().isCompatible(validData))
                i.value()->run(validData);
            ++i;
        }
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
 * @param[in] req All data required by the pipeline.
 */
void PipelineDriver::_createDataBlobs(const DataRequirements& req)
{
    /* Create a union of all data requirements */
    QSet<QString> allData = req.serviceData() + req.streamData();

    /* Iterate over data requirements to create blobs if they don't exist */
    foreach (QString type, allData) {
        if (  ! _dataHash.contains(type) )
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
 * @param[in] name The type of the data client to create.
 * @param[in] config The application's configuration object.
 */
void PipelineDriver::_createDataClient(QString name, Config* config)
{
    /* Check configuration object exists */
    if (config == NULL)
        throw QString("Configuration not set: call setDataClient() first.");

    /* Get the configuration address */
    Config::TreeAddress_t address;
    address.append(Config::NodeId_t("clients", ""));
    address.append(QPair<QString, QString>("client", name));
    ConfigNode element = config->get(address);

    /* Create the required data client */
    if (name == "FileDataClient") {
        _dataClient = new FileDataClient(element, _adapterFactory, _pipelines.keys());
    }
    else {
        throw QString("Unknown data client type: ").arg(name);
    }
}

/**
 * @details
 * Private method to find the data requirements of the given pipeline.
 * This is called by _initialisePipelines().
 */
void PipelineDriver::_determineDataRequirements(AbstractPipeline* pipeline)
{
    /* Check for an empty pipeline */
    if (pipeline->requiredDataAll() == DataRequirements())
        throw QString("Empty pipelines are not supported.");

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

    /* Store the remote data requirements for this pipeline */
    _pipelines.insert(pipeline->requiredDataRemote(), pipeline);

    /* Accumulate all data requirements ready to create data blobs */
    _reqDataAll += pipeline->requiredDataAll();
}

/**
 * @details
 * Private method to initialise the pipelines by iterating over them,
 * creating their modules and finding out their data requirements.
 * This is called by start().
 */
void PipelineDriver::_initialisePipelines()
{
    /* Check for at least one registered pipeline */
    if (_registeredPipelines.isEmpty())
        throw QString("No pipelines.");

    foreach (AbstractPipeline* pipeline, _registeredPipelines) {
        /* Set the module factory and pipeline driver */
        pipeline->setModuleFactory(_moduleFactory);
        pipeline->setPipelineDriver(this);

        /* Initialise the pipeline (create modules) */
        pipeline->init();

        /* Add the required data to the data requirements */
        _determineDataRequirements(pipeline);
    }
}

} // namespace pelican

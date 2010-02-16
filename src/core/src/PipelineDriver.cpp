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
 * Sets (and creates) the given data client based on the named argument.
 * Throws an exception of type QString if the data client is unknown.
 *
 * The recognised values are:
 * - FileDataClient
 *
 * @param[in] name The type of the data client to create.
 */
void PipelineDriver::setDataClient(QString name, Config* config)
{
    /* Get the configuration address */
    Config::TreeAddress_t address;
    address.append(Config::NodeId_t("clients", ""));
    address.append(QPair<QString, QString>("client", name));
    ConfigNode element = config->get(address);

    /* Create the required data client */
    if (name == "FileDataClient") {
        QList<DataRequirements> list; // TODO
        _dataClient = new FileDataClient(element, _adapterFactory, list);
    }
    else {
        throw QString("Unknown data client type: ").arg(name);
    }
}

/**
 * @details
 * Iterates over all registered pipelines to determine the required data and
 * starts the data flow through the pipelines.
 */
void PipelineDriver::start()
{
    /* Initialise the pipelines */
    _initialisePipelines();

    /* Start the pipeline driver */
    if (_dataClient == NULL)
        throw QString("No data client set.");

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
    QSet<QString> allData = req.serviceData() & req.streamData();

    /* Iterate over the data requirements to create blobs if they don't exist */
    foreach (QString type, allData) {
        if (  ! _dataHash.contains(type) )
            _dataHash.insert(type, _blobFactory->create(type));
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
    if (pipeline->dataRequired() == DataRequirements())
        throw QString("Empty pipelines are not supported.");

    /* Check that the set of stream data required for this pipeline does not
     * intersect the set of stream data required by another.
     * Data is not currently copied, so this ensures that two pipelines do not
     * try to modify the same data. */
    QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
    while (i != _pipelines.end()) {
        if ((i.key().streamData() & pipeline->dataRequired().streamData()).size() > 0) {
            throw QString("Multiple pipelines requiring the same stream data are not currently supported.");
        }
        ++i;
    }

    _pipelines.insert(pipeline->dataRequired(), pipeline);
    _requiredData += pipeline->dataRequired();
}

/**
 * @details
 * Private method to initialises the pipelines by iterating over them,
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

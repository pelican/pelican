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
PipelineDriver::PipelineDriver (
        DataBlobFactory* blobFactory,
        ModuleFactory* moduleFactory,
        DataClientFactory* clientFactory
        )
    :  _blobFactory(blobFactory), _moduleFactory(moduleFactory), _clientFactory(clientFactory)
{
    /* Initialise member variables */
    _run = false;
    _dataClient = NULL;
    Q_ASSERT(_moduleFactory != 0 );
    Q_ASSERT(_blobFactory != 0 );
    Q_ASSERT(_clientFactory != 0 );
}

/**
 * @details
 * PipelineDriver destructor. This deletes all the registered pipelines,
 * the data blobs and the data client.
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
 * Registered pipelines will be deleted when the class is destroyed.
 *
 * @param[in] pipeline Pointer to the allocated pipeline.
 */
void PipelineDriver::registerPipeline(AbstractPipeline *pipeline)
{
    /*
    if( pipeline->requiredDataAll().size() == 0 ) {
        delete pipeline;
        throw(QString("pipeline requiring no data has been passed") );
    }
    */
    pipeline->setModuleFactory(_moduleFactory);
    pipeline->setPipelineDriver(this);
    pipeline->init();
    _allDataRequirements.append(pipeline->requiredDataRemote());
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
    /* Initialise the pipelines and determine their data requirements */
    _initialisePipelines();

    /* Create the data blobs */
    _createDataBlobs(_reqDataAll);

    /* Create the data client */
    _createDataClient(_dataClientName);

    /* Enter main program loop */
    _run = true;
    while (_run) {
        /* Get the data from the client */
        QHash<QString, DataBlob*> validData = _dataClient->getData(_dataHash);

        /* Check for empty data */
        if (validData.isEmpty()) {
//            _run = false;
            throw QString("No data returned from client.");
        }

        /* Run all the pipelines compatible with this data hash */
        QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
        while (i != _pipelines.end()) {
            if (i.key().isCompatible(validData))
                i.value()->run(_dataHash);
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
 * This private method is called by start().
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
 * @param[in] type The type of the data client to create.
 */
void PipelineDriver::_createDataClient(QString type)
{
    _dataClient = _clientFactory->create(type, dataRequirements() );
}

/*
    Config::TreeAddress_t address;
    address.append(Config::NodeId_t("clients", ""));
    address.append(QPair<QString, QString>(type, ""));
    ConfigNode element = config->get(address);

    if (type == "FileDataClient") {
        _dataClient = new FileDataClient(element, _adapterFactory, _pipelines.keys());
    }
    if (type == "{PelicanServerClient") {
        _dataClient = new PelicanServerClient(element, _adapterFactory, _pipelines.keys());
    }
    else {
        throw QString("Unknown data client type: ").arg(type);
    }
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
    // Loop over data requirements for each pipeline
    /*
    foreach (DataRequirements& req, dataRequirements) {
        // Create a union of required data types for this pipeline 
        QSet<QString> allDataReq = req.externalData();

        // Loop over each data type to set up the adapters for each pipeline
        foreach (const QString& type, allDataReq) 
        {
            if( ! adapterNames.contains(type) )
                throw("Unable to find adapter for data type \"" + type + "\"");
            AbstractAdapter* adapter = adapterFactory->create(_adapterNames.value(type), "");
            req->setAdapter( type, adapter );
            _adapters.insert(type, adapter);
        }
    }
    */

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

    /* Accumulate all data requirements ready to create remote data blobs */
    _reqDataAll += pipeline->requiredDataRemote();
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
        //pipeline->setModuleFactory(_moduleFactory);
        //pipeline->setPipelineDriver(this);

        /* Initialise the pipeline (create modules) */
        //pipeline->init();

        /* Add the required data to the data requirements */
        _determineDataRequirements(pipeline);
    }
}

} // namespace pelican

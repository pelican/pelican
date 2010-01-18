#include <QString>
#include <QtGlobal>
#include <QtDebug>
#include "utility/memCheck.h"
#include "PipelineDriver.h"
#include "AbstractPipeline.h"
#include "AbstractDataClient.h"
#include "ModuleFactory.h"
#include "data/DataBlob.h"

namespace pelican {

/**
 * @details
 * PipelineDriver constructor, which takes the command line arguments
 * for initialisation.
 */
PipelineDriver::PipelineDriver()
{
    /* Initialise member variables */
    _run = false;
    _dataClient = NULL;
    _moduleFactory = NULL;
}

/**
 * @details
 * PipelineDriver destructor. This deletes all the registered pipelines.
 */
PipelineDriver::~PipelineDriver()
{
    foreach (AbstractPipeline* pipeline, _registeredPipelines) {
        delete pipeline;
    }
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
 * Sets the data client.
 */
void PipelineDriver::setDataClient(AbstractDataClient *client)
{
    _dataClient = client;
}

/**
 * @details
 * Sets the module factory.
 */
void PipelineDriver::setModuleFactory(ModuleFactory *moduleFactory)
{
    _moduleFactory = moduleFactory;
}

/**
 * @details
 * Iterates over all registered pipelines to determine the required data and
 * starts the data flow through the pipelines.
 */
void PipelineDriver::start()
{
    /* Iterate over the registered pipelines to initialise them
     * (to create their modules) and find the data requirements */
    foreach (AbstractPipeline* pipeline, _registeredPipelines) {
        /* Set the module factory and pipeline driver */
        pipeline->setModuleFactory(_moduleFactory);
        pipeline->setPipelineDriver(this);

        /* Initialise the pipeline (create modules) */
        pipeline->init();

        /* Add the required data to the data requirements */
        _registerPipeline(pipeline);
    }

    /* Check for at least one pipeline */
    if (_pipelines.isEmpty())
        throw QString("No pipelines.");

    /* Start the pipeline driver */
    if (_dataClient == NULL)
        throw QString("No data client set.");

    /* Enter main program loop */
    _run = true;
    while (_run) {
        /* Get the data from the client */
        QHash<QString, DataBlob*> data = _dataClient->getData(_requiredData);

        /* Check for empty data */
        if (data.isEmpty())
            throw QString("No data returned from client.");

        /* Run all the pipelines compatible with this data hash */
        QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
        while (i != _pipelines.end()) {
            if (i.key().isCompatible(data))
                i.value()->run(data);
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
 * Private method to find the data requirements of the given pipeline.
 */
void PipelineDriver::_registerPipeline(AbstractPipeline* pipeline)
{
    /* Check for an empty pipeline */
    if (pipeline->dataRequired() == DataRequirements())
        throw QString("Empty pipelines are not supported.");

    /* Check that the stream data required for this pipeline is different
     * from all the others.
     * Data is not currently copied, so this ensures that two pipelines do not
     * try to modify the same data. */
    QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
    while (i != _pipelines.end()) {
        if (i.key().streamData() == pipeline->dataRequired().streamData()) {
            throw QString("Multiple pipelines requiring the same stream data are not currently supported.");
        }
        ++i;
    }

    _pipelines.insert(pipeline->dataRequired(), pipeline);
    _requiredData += pipeline->dataRequired();
}

} // namespace pelican

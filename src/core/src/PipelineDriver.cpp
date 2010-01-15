#include <QString>
#include <QtGlobal>
#include <QtDebug>
#include "utility/memCheck.h"
#include "PipelineDriver.h"
#include "AbstractPipeline.h"
#include "DataClient.h"
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
    _dataClient = NULL;
}

/**
 * @details
 * PipelineDriver destructor. This deletes all the registered pipelines.
 */
PipelineDriver::~PipelineDriver()
{
    foreach (AbstractPipeline* pipeline, _pipelines) {
        delete pipeline;
    }
}

/**
 * @details
 * Registers the pipeline with the driver.
 */
void PipelineDriver::registerPipeline(AbstractPipeline *pipeline)
{
    if (pipeline->dataRequired() == DataRequirements()) {
        delete pipeline;
        throw QString("Empty pipelines are not supported.");
    }

    /* Check that the stream data required for this pipeline is different
     * from all the others.
     * Data is not currently copied, so this ensures that two pipelines do not
     * try to modify the same data. */
    QMultiHash<DataRequirements, AbstractPipeline*>::iterator i = _pipelines.begin();
    while (i != _pipelines.end()) {
        if (i.key().streamData() == pipeline->dataRequired().streamData()) {
            delete pipeline;
            throw QString("Multiple pipelines requiring the same stream data are not currently supported.");
        }
        ++i;
    }

    _pipelines.insert(pipeline->dataRequired(), pipeline);
    _requiredData += pipeline->dataRequired();
}

/**
 * @details
 * Starts the data flow through the pipelines.
 */
void PipelineDriver::start()
{
    /* Check for at least one pipeline */
    if (_pipelines.isEmpty()) {
        throw QString("No pipelines.");
    }

    /* Start the pipeline driver */
    if (_dataClient == NULL)
        throw QString("No data client set.");

    _run = true;
    while (_run) {
        /* Get the data from the client */
        QHash<QString, DataBlob*> data = _dataClient->getData(_requiredData);

        /* Check for empty data */
        if (data.isEmpty()) {
            throw QString("No data returned from client.");
        }

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
 * Sets the data client.
 */
void PipelineDriver::setDataClient(AbstractDataClient *client)
{
    _dataClient = client;
}

} // namespace pelican

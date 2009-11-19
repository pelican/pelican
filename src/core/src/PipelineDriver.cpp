#include "PipelineDriver.h"
#include "DataRequirements.h"
#include "AbstractPipeline.h"
#include "DataClient.h"
#include "data/DataBlob.h"

#include <QString>
#include <QtGlobal>
#include <QtDebug>

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
 * PipelineDriver destructor.
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
    _pipelines.insert(pipeline->dataRequired(), pipeline);
    _requiredData += pipeline->dataRequired();
}

/**
 * @details
 * Returns a reference to the module factory.
 */
ModuleFactory& PipelineDriver::moduleFactory() const
{
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
    _run = true;
    while (_run) {
        /* Get the data from the client */
        QHash<QString, DataBlob*> data = _dataClient->getData(_requiredData);
        if (data.isEmpty()) {
            throw QString("No data returned from client.");
        }
        DataRequirements returnedData;
        returnedData.setStreamData(data.keys().toSet());

        QList<AbstractPipeline*> list = _pipelines.values(returnedData);
        foreach (AbstractPipeline* pipeline, list) {
            pipeline->run();
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
void PipelineDriver::setDataClient(const QString& clientName)
{
}

} // namespace pelican


#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

#include "data/DataRequirements.h"
#include <QMultiHash>

/**
 * @file PipelineDriver.h
 */

namespace pelican {

class AbstractPipeline;
class ModuleFactory;
class DataClient;

/**
 * @class PipelineDriver
 *
 * @brief Pipeline driver class.
 *
 * @details
 * This class controls the data flow through the pipelines.
 * The pipeline driver also takes ownership of the pipelines and is
 * responsible for deleting them.
 */
class PipelineDriver
{
    public:
        /// Constructs a new pipeline driver.
        PipelineDriver();

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        /// Registered pipelines will be deleted when the class is destroyed.
        void registerPipeline(AbstractPipeline *pipeline);
    
        /// Starts the data flow through the pipelines.
        void start();

        /// Stops the data flow through the pipelines.
        void stop();

        /// Sets the data client.
        void setDataClient(DataClient *client);

    private: /* Methods */

    private:
        DataClient *_dataClient;
        DataRequirements _requiredData;
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;
        bool _run;
};

} // namespace pelican

#endif // PIPELINEDRIVER_H 

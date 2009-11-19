#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

#include "DataRequirements.h"
#include <QMultiHash>

/**
 * @file PipelineDriver.h
 */

/**
 * @class PipelineDriver
 *
 * @brief Pipeline driver class.
 *
 * @details
 * This class controls the data flow through the pipelines.
 * It is also responsible for the initialisation of the pipeline module
 * factories.
 * This class takes ownership of the pipelines and is responsible for
 * deleting them.
 */

namespace pelican {

class AbstractPipeline;
class ModuleFactory;
class DataClient;

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
    
        /// Returns a reference to the module factory.
        ModuleFactory& moduleFactory() const;
    
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

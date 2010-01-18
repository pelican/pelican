#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

#include "data/DataRequirements.h"
#include <QMultiHash>
#include <QList>

/**
 * @file PipelineDriver.h
 */

namespace pelican {

class AbstractPipeline;
class ModuleFactory;
class AbstractDataClient;

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
    private: /* Data */
        AbstractDataClient* _dataClient;
        ModuleFactory* _moduleFactory;
        DataRequirements _requiredData;
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;
        QList<AbstractPipeline*> _registeredPipelines;
        bool _run;

    public:
        /// Constructs a new pipeline driver.
        PipelineDriver();

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        void registerPipeline(AbstractPipeline *pipeline);
    
        /// Sets the data client.
        void setDataClient(AbstractDataClient *client);

        /// Sets the module factory.
        void setModuleFactory(ModuleFactory *moduleFactory);

        /// Starts the data flow through the pipelines.
        void start();

        /// Stops the data flow through the pipelines.
        void stop();

    private:
        /// Find the data requirements of the given \p pipeline.
        void _registerPipeline(AbstractPipeline *pipeline);
};

} // namespace pelican

#endif // PIPELINEDRIVER_H 

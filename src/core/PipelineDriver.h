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
class DataBlobFactory;

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
        friend class PipelineDriverTest;

    private: /* Data */
        /// Pointer to the data client.
        AbstractDataClient* _dataClient;

        /// Pointer to the module factory.
        AdapterFactory* _adapterFactory;

        /// A pointer to the data blob factory.
        DataBlobFactory* _blobFactory;

        /// Pointer to the module factory.
        ModuleFactory* _moduleFactory;

        /// All data required by all pipelines.
        DataRequirements _reqDataAll;

        /// Hash of pipelines with known remote data requirements.
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;

        /// List of registered pipelines owned by the driver.
        QList<AbstractPipeline*> _registeredPipelines;

        /// Flag to run the pipeline driver.
        bool _run;

        /// The hash of data returned by the getData() method.
        QHash<QString, DataBlob*> _dataHash;

        /// The name of the data client.
        QString _dataClientName;

        /// Pointer to the configuration object.
        Config *_config;

    public:
        /// Constructs a new pipeline driver.
        PipelineDriver(AdapterFactory* adapterFactory,
                DataBlobFactory* blobFactory, ModuleFactory* moduleFactory);

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        void registerPipeline(AbstractPipeline *pipeline);
    
        /// Sets the data client.
        void setDataClient(QString name, Config* config);

        /// Starts the data flow through the pipelines.
        void start();

        /// Stops the data flow through the pipelines.
        void stop();

    private:
        /// Creates all the data blobs required by the pipeline.
        void _createDataBlobs(const DataRequirements&);

        /// Creates the data client.
        void _createDataClient(QString name, Config* config);

        /// Find the data requirements of the given \p pipeline.
        void _determineDataRequirements(AbstractPipeline *pipeline);

        /// Initialise pipelines.
        void _initialisePipelines();
};

} // namespace pelican

#endif // PIPELINEDRIVER_H 

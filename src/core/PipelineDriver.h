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

        /// Pointer to the module factory.
        ModuleFactory* _moduleFactory;

        /// A pointer to the data blob factory.
        DataBlobFactory* _blobFactory;

        /// Data required by all the pipelines.
        DataRequirements _requiredData;

        /// Hash of pipelines with known data requirements.
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;

        /// List of registered pipelines owned by the driver.
        QList<AbstractPipeline*> _registeredPipelines;

        /// Flag to run the pipeline driver.
        bool _run;

        /// The hash of data returned by the getData() method.
        QHash<QString, DataBlob*> _dataHash;

    public:
        /// Constructs a new pipeline driver.
        PipelineDriver(DataBlobFactory* blobFactory, AdapterFactory* adapterFactory);

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        void registerPipeline(AbstractPipeline *pipeline);
    
        /// Sets the data client.
        void setDataClient(AbstractDataClient *client);

        /// Sets the data client.
        void setDataClient(QString name, Config* config);

        /// Sets the module factory.
        void setModuleFactory(ModuleFactory *moduleFactory);

        /// Starts the data flow through the pipelines.
        void start();

        /// Stops the data flow through the pipelines.
        void stop();

    private:
        /// Find the data requirements of the given \p pipeline.
        void _determineDataRequirements(AbstractPipeline *pipeline);

        /// Initialise pipelines.
        void _initialisePipelines();

        /// Creates all the data blobs required by the pipeline.
        void _createDataBlobs(const DataRequirements&);

        /// Returns a pointer to the data blob factory.
        DataBlobFactory* blobFactory() {return _blobFactory;}
};

} // namespace pelican

#endif // PIPELINEDRIVER_H 

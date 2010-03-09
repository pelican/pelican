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
class AbstractAdapter;
class DataBlobFactory;
class DataClientFactory;
class AdapterFactory;

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

        /// A pointer to the data blob factory.
        DataBlobFactory* _blobFactory;

        /// Pointer to the module factory.
        ModuleFactory* _moduleFactory;

        /// Pointer to the module factory.
        DataClientFactory* _clientFactory;

        /// All data required by all pipelines.
        DataRequirements _reqDataAll;

        /// Hash of pipelines with known remote data requirements.
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;

        /// list of all requirements objects form each pipeline
        QList<DataRequirements> _allDataRequirements;

        /// List of registered pipelines owned by the driver.
        QList<AbstractPipeline*> _registeredPipelines;

        /// Flag to run the pipeline driver.
        bool _run;

        /// The hash of data returned by the getData() method.
        QHash<QString, DataBlob*> _dataHash;

        /// The name of the data client.
        QString _dataClientName;

        /// All the adapters created for each data type.
        QHash<QString, AbstractAdapter*> _adapters;

        /// The adapter names required for each data type.
        QHash<QString, QString> _adapterNames;

    public:
        /// Constructs a new pipeline driver.
        PipelineDriver( DataBlobFactory* blobFactory, ModuleFactory* moduleFactory, 
                DataClientFactory* clientFactory );

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        void registerPipeline(AbstractPipeline *pipeline);

        /// Sets the data client.
        void setDataClient(QString name);

        /// Starts the data flow through the pipelines.
        void start();

        /// return the dataRequirements lists for each pipeline
        const QList<DataRequirements>& dataRequirements() const;

        /// Stops the data flow through the pipelines.
        void stop();

    private:
        /// Creates all the data blobs required by the pipeline.
        void _createDataBlobs(const DataRequirements&);

        /// Creates the data client.
        void _createDataClient(QString name);

        /// Find the data requirements of the given \p pipeline.
        void _determineDataRequirements(AbstractPipeline *pipeline);

        /// Initialise pipelines.
        void _initialisePipelines();
};

} // namespace pelican

#endif // PIPELINEDRIVER_H

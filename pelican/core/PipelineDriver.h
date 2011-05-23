#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

/**
 * @file PipelineDriver.h
 */

#include "pelican/data/DataBlob.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/FactoryGeneric.h"
#include <QtCore/QMultiHash>
#include <QtCore/QList>

namespace pelican {

class AbstractPipeline;
class AbstractDataClient;
class AbstractAdapter;
class DataClientFactory;
class OutputStreamManager;

/**
 * @ingroup c_core
 *
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
    private:
        friend class PipelineDriverTest;

    private:
        /// Pointer to the data client.
        AbstractDataClient* _dataClient;

        /// A pointer to the data blob factory.
        FactoryGeneric<DataBlob>* _blobFactory;

        /// Pointer to the module factory.
        FactoryConfig<AbstractModule>* _moduleFactory;

        OutputStreamManager* _osmanager;

        /// Pointer to the module factory.
        DataClientFactory* _clientFactory;

        /// Hash of pipelines with known remote data requirements.
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;

        /// List of all requirements objects from each pipeline.
        QList<DataRequirements> _allDataReq;

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
        PipelineDriver(FactoryGeneric<DataBlob>* blobFactory,
                FactoryConfig<AbstractModule>* moduleFactory,
                DataClientFactory* clientFactory,
                OutputStreamManager* outputManager);

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        void registerPipeline(AbstractPipeline *pipeline);

        /// Sets the data client.
        void setDataClient(QString name);

        /// Starts the data flow through the pipelines.
        void start();

        /// Stops the data flow through the pipelines.
        void stop();

        /// return true if the driver main loop is running
        bool isRunning() const { return _run; }

    private:
        /// Checks that the data requirements of all pipelines are compatible.
        void _checkDataRequirements();
};

} // namespace pelican
#endif // PIPELINEDRIVER_H

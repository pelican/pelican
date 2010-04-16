#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

#include "pelican/data/DataRequirements.h"
#include "pelican/utility/Factory.h"
#include <QMultiHash>
#include <QList>

/**
 * @file PipelineDriver.h
 */

namespace pelican {

class AbstractModule;
class AbstractPipeline;
class AbstractDataClient;
class AbstractAdapter;
class DataBlob;
class DataClientFactory;

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
    private:
        friend class PipelineDriverTest;

    private:
        /// Pointer to the data client.
        AbstractDataClient* _dataClient;

        /// A pointer to the data blob factory.
        Factory<DataBlob>* _blobFactory;

        /// Pointer to the module factory.
        Factory<AbstractModule>* _moduleFactory;

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

        /// FIXME Flag set ignore empty hash.
        bool _ignoreEmptyHash;

    public:
        /// Constructs a new pipeline driver.
        PipelineDriver( Factory<DataBlob>* blobFactory,
                Factory<AbstractModule>* moduleFactory,
                DataClientFactory* clientFactory );

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

        /// Sets the option to ignore an empty hash.
        void setIgnoreEmptyHash(bool value) {_ignoreEmptyHash = value;}

    private:
        /// Checks that the data requirements of all pipelines are compatible.
        void _checkDataRequirements();
};

} // namespace pelican

#endif // PIPELINEDRIVER_H

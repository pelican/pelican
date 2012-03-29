#ifndef PIPELINEDRIVER_H
#define PIPELINEDRIVER_H

/**
 * @file PipelineDriver.h
 */

#include "PipelineSwitcher.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/TypeCounter.h"
#include "pelican/utility/FactoryGeneric.h"
#include <QtCore/QMultiHash>
#include <QtCore/QList>
#include <QtCore/QVector>

namespace pelican {

class AbstractPipeline;
class AbstractDataClient;
class AbstractAdapter;
class DataClientFactory;
class OutputStreamManager;
class PipelineSwitcher;
class DataBlobBuffer;
class Config;

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
        OutputStreamManager* _osmanager;

        /// Configuration root node.
        const Config* _config;
        Config::TreeAddress _base; 

        /// Pointer to the data client.
        AbstractDataClient* _dataClient;

        /// A pointer to the data blob factory.
        FactoryGeneric<DataBlob>* _blobFactory;

        /// Pointer to the module factory.
        FactoryConfig<AbstractModule>* _moduleFactory;


        /// Pointer to the module factory.
        DataClientFactory* _clientFactory;

        /// Hash of pipelines with known remote data requirements.
        QMultiHash<DataRequirements, AbstractPipeline*> _pipelines;

        /// List of all requirements objects from each pipeline.
        QList<DataRequirements> _allDataReq;

        /// Circular Buffers for retaining DataBlob history
        QHash<QString,DataBlobBuffer*> _dataBuffers;

        /// List of registered and active pipelines owned by the driver.
        QList<AbstractPipeline*> _registeredPipelines;

        /// size of history buffers
        QHash<QString, TypeCounter<unsigned int> > _history;

        /// Pipeline switching objects
        QList<PipelineSwitcher> _switchers;
        QHash<AbstractPipeline*, PipelineSwitcher*> _switcherMap;
        QVector<AbstractPipeline*> _deactivateQueue;

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
                OutputStreamManager* outputManager, 
                const Config* config,
                const Config::TreeAddress& pipelineConfig );

        /// Destroys the pipeline driver.
        ~PipelineDriver();

        /// Registers the pipeline with the driver.
        //  and activates it to be run at start()
        void registerPipeline(AbstractPipeline *pipeline);

        /// A a switchable block of pipelines, The next will be activated each
        //  time the current pipeline is deactivated
        void addPipelineSwitcher(const PipelineSwitcher& switcher);

        /// Sets the data client.
        void setDataClient(QString name);
        void setDataClient(AbstractDataClient* client);

        /// Starts the data flow through the pipelines.
        void start();

        /// Stops the data flow through the pipelines.
        void stop();

        /// return true if the driver main loop is running
        bool isRunning() const { return _run; }

        // queue a registered pipeline for deactivation
        void deactivatePipeline(AbstractPipeline*);

        // return the named configuration node, relative to the base adress
        ConfigNode config( const QString& pipeline, const QString& name="") const;

    private:
        /// deactivate a registered pipeline
        void _deactivatePipeline(AbstractPipeline*);

        /// Checks that the data requirements of all pipelines are compatible.
        void _checkDataRequirements();

        /// register a pipeline
        void _registerPipeline(AbstractPipeline*);

        /// activates a pipeline, ensuring datablobs are available.
        void _activatePipeline(AbstractPipeline*);

        /// adjust internals to a new history size
        void _updateHistoryBuffers();
};

} // namespace pelican
#endif // PIPELINEDRIVER_H

#include "pelican/core/PipelineDriver.h"

#include "pelican/core/DataClientFactory.h"
#include "pelican/core/AbstractDataClient.h"
#include "pelican/core/FileDataClient.h"
#include "pelican/core/AbstractPipeline.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataBlobBuffer.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/core/PipelineSwitcher.h"

#include <QtCore/QString>
#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <iostream>


namespace pelican {

/**
 * @details
 * PipelineDriver constructor, which takes pointers to the allocated factories.
 */
PipelineDriver::PipelineDriver(FactoryGeneric<DataBlob>* blobFactory,
        FactoryConfig<AbstractModule>* moduleFactory,
        DataClientFactory* clientFactory, OutputStreamManager* osmanager,
        const Config* config,
        const Config::TreeAddress& pipelineConfig )
    : _osmanager(osmanager), _config(config), _base(pipelineConfig)
{
    // Initialise member variables.
    _run = false;
    _dataClient = NULL;

    // Store pointers to factories.
    _blobFactory = blobFactory;
    _moduleFactory = moduleFactory;
    _clientFactory = clientFactory;
    Q_ASSERT(_config != 0 );
    Q_ASSERT(_blobFactory != 0 );
    Q_ASSERT(_moduleFactory != 0 );
    Q_ASSERT(_clientFactory != 0 );
}

/**
 * @details
 * PipelineDriver destructor. This deletes all the registered pipelines.
 */
PipelineDriver::~PipelineDriver()
{
    // Delete the pipelines.
    foreach (AbstractPipeline* pipeline, _registeredPipelines) {
        delete pipeline;
    }
    _registeredPipelines.clear();
    foreach(DataBlobBuffer* buffer, _dataBuffers ) {
        delete buffer;
    }
    _dataBuffers.clear();
}

/**
 * @details
 * Public interface to register the given \p pipeline with the driver.
 * The pipeline is initialised when this method is called.
 *
 * Registered pipelines will be deleted when the class is destroyed.
 *
 * @param[in] pipeline Pointer to the allocated pipeline.
 */
void PipelineDriver::registerPipeline(AbstractPipeline *pipeline)
{
    _registerPipeline(pipeline);
    _activatePipeline(pipeline);
}

void PipelineDriver::_registerPipeline(AbstractPipeline *pipeline)
{
    // Check the pipeline exists.
    if (!pipeline)
        throw QString("PipelineDriver::registerPipeline(): Null pipeline.");

    // Store the pointer to the registered pipeline.
    _registeredPipelines.append(pipeline);

    // Set up and initialise the pipeline.
    pipeline->setBlobFactory(_blobFactory);
    pipeline->setModuleFactory(_moduleFactory);
    pipeline->setPipelineDriver(this);
    pipeline->setOutputStreamManager(_osmanager);
    // determine any pipeline configuration
    
    pipeline->init();

    // Store the remote data requirements.
    _allDataReq.append(pipeline->requiredDataRemote());
}

ConfigNode PipelineDriver::config( const QString& tag, const QString& name ) const {
    if (!_config) throw QString("PipelineDriver configuration not set");
    Config::TreeAddress address(_base);
    address << Config::NodeId(tag, name);
    return _config->get(address);
}

void PipelineDriver::_activatePipeline(AbstractPipeline *pipeline)
{
     if( pipeline ) {
        foreach ( const QString& type, pipeline->requiredDataRemote().allData() ) {
            // add history requirements
            _history[type].add( pipeline->historySize(type) );
            // create a history buffer for each type
            if( ! _dataBuffers.contains(type) ) {
                // ensure buffer exists
                _dataBuffers.insert(type, new DataBlobBuffer );
                _dataHash.insert(type,NULL);
            }
            unsigned int max=_history[type].max();
            if( max > (unsigned int)_dataBuffers[type]->size() ) { // scale up to required size
                for(unsigned int i=_dataBuffers[type]->size(); i<max; ++i ) {
                    _dataBuffers[type]->addDataBlob(_blobFactory->create(type));
                }
            }
            else if( max < (unsigned int)_dataBuffers[type]->size() ) {
                // shrink the history buffer
                _dataBuffers[type]->shrink(max);
            }
        }
        _pipelines.insert(pipeline->requiredDataRemote(), pipeline);
     }
}


void PipelineDriver::deactivatePipeline(AbstractPipeline *pipeline)
{
    if( pipeline ) {
        // queue the pipeline to be deactivated when it is safe to do so
        _deactivateQueue.append(pipeline);
    }
}

void PipelineDriver::_deactivatePipeline(AbstractPipeline *pipeline)
{
    if( pipeline ) {
        // put reqs in a temporary to work around broken QMultiHash headers
        // in Qt 4.2 which don't accept a const key
        DataRequirements reqs = pipeline->requiredDataRemote();
        _pipelines.remove(reqs, pipeline);
         // adjust history buffers
         foreach ( const QString& type, reqs.allData() ) {
              if( _history.contains(type) ) {
                 _history[type].remove( pipeline->historySize(type) );
                if( _history[type].isEmpty() || _history[type].max() == 0 ) {
                    // remove the buffer completely when no longer needed
                    delete _dataBuffers[type];
                    _dataBuffers.remove(type);
                    _dataHash.remove(type);
                }
            }
         }
         // if the pipeline is in a switcher then get the next one
         if( _switcherMap.contains(pipeline) ) {
             AbstractPipeline* next = _switcherMap[pipeline]->next();
             // mark the next pipeline against the switcher
             if( next != pipeline ) {
                 _switcherMap[next] = _switcherMap[pipeline];
                 _switcherMap.remove(pipeline);
             }
             // now activate the new pipeline
             _activatePipeline(next);
         }
     }
}

void PipelineDriver::addPipelineSwitcher(const PipelineSwitcher& switcher)
{
    _switchers.push_back(switcher);
     PipelineSwitcher* sw = &_switchers.last(); // pointer to local switcher copy

     AbstractPipeline* next = sw->next();
     DataRequirements reqs = next->requiredDataRemote();

     // register all the pipelines in the switcher
     foreach( AbstractPipeline* pipe, switcher.pipelines() ) {
        if( pipe->requiredDataRemote() != reqs )
                throw( QString("PipelineDriver: Pipelines with different Data requirements in"
                               " the same switcher is not supported") );
        _registerPipeline(pipe);
     }

     // activate the first
     _switcherMap[next] = sw;
     _activatePipeline(next);
}

/**
 * @details
 * Sets the given data client based on the named argument.
 * The client is only created after the pipelines have been initialised,
 * when start() is called.
 *
 * @param[in] name The type of the data client to create.
 */
void PipelineDriver::setDataClient(QString name)
{
    _dataClientName = name;
}

void PipelineDriver::setDataClient( AbstractDataClient* client ) {
    _dataClient = client;
}

/**
 * @details
 * Iterates over all registered pipelines to determine the required data and
 * starts the data flow through the pipelines.
 *
 * This public method is called by PipelineApplication start().
 */
void PipelineDriver::start()
{
    // Check for at least one registered pipeline.
    if (_registeredPipelines.isEmpty())
        throw QString("PipelineDriver::start(): No pipelines registered.");

    // Create the data client.
    if (!_dataClientName.isEmpty() && _dataClient == NULL )
        _dataClient = _clientFactory->create(_dataClientName, _allDataReq);

    // Check the data requirements.
    _checkDataRequirements();

    // Enter main program loop.
    _run = true;
    QString lastError;
    while (_run) {
        // Get the data from the client.
        QHash<QString, DataBlob*> validData;
        foreach( const QString& type, _dataHash.keys() ) {
            _dataHash[type]=_dataBuffers[type]->next();
        }
        try {
            if (_dataClient) {
                validData = _dataClient->getData(_dataHash);
            }
        }
        catch(const QString& e)
        {
            // log the error and keep going
            if( e != lastError )
                std::cerr << e.toStdString() << std::endl;
            lastError = e;
            continue;
        }
        lastError = "";

        // Run all the pipelines compatible with this data hash.
        bool ranPipeline = false;
        QMultiHash<DataRequirements, AbstractPipeline*>::iterator pipe;
        for (pipe = _pipelines.begin(); pipe != _pipelines.end(); ++pipe) {
            if (pipe.key().isCompatible(validData)) {
                ranPipeline = true;
                pipe.value()->exec(_dataHash);
            }
        }

        // deactivate any pipelines
        while( _deactivateQueue.size() > 0 ) {
             _deactivatePipeline(_deactivateQueue[0]);
             _deactivateQueue.pop_front();
        }

        // Check if no pipelines were run.
        if (!ranPipeline) {
            QString msg;
            foreach( const QString& d, validData.keys() ) {
                msg += " " + d;
            }
            throw QString("PipelineDriver::start(): received data incompatible with the pipelines:"
                                + msg );
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
 * Private method to find the data requirements of the given pipeline.
 * This is called by start().
 */
void PipelineDriver::_checkDataRequirements()
{
    if (!_dataClient)
        return;

    /* Check that the set of stream data required for each pipeline does not
     * intersect the set of stream data required by another.
     * Data is not currently copied, so this ensures that two pipelines do not
     * try to modify the same data. */
    DataRequirements totalReq;
    foreach (DataRequirements req, _dataClient->dataRequirements()) {
#ifdef BROKEN_QT_SET_HEADER
        QSet<QString> temp = totalReq.streamData();
        if ((temp & req.streamData()).size() > 0) {
#else
        if ((totalReq.streamData() & req.streamData()).size() > 0) {
#endif
            throw QString("Multiple pipelines requiring the same remote stream data are not supported.");
        }
        totalReq += req;
    }
}

} // namespace pelican

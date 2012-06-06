#include <QtCore/QtGlobal>
#include "pelican/modules/AbstractModule.h"
#include "pelican/core/AbstractPipeline.h"
#include "pelican/core/PipelineApplication.h"
#include "pelican/core/PipelineDriver.h"
#include "pelican/data/DataBlobBuffer.h"
#include "pelican/output/OutputStreamManager.h"

namespace pelican {

/**
 * @details
 * AbstractPipeline constructor.
 */
AbstractPipeline::AbstractPipeline()
{
    // Initialise members.
    _blobFactory = NULL;
    _moduleFactory = NULL;
    _pipelineDriver = NULL;
}

/**
 * @details
 * AbstractPipeline destructor. This is implemented as a virtual so that
 * subclasses of AbstractPipeline will be destroyed cleanly.
 */
AbstractPipeline::~AbstractPipeline()
{
    QHash<QString, QList<DataBlob*>* >::iterator it;
    for (it = _streamHistory.begin(); it != _streamHistory.end(); ++it) {
        delete it.value();
    }
}

/**
 * @details
 * Creates a new data blob using the pipeline application's data blob factory.
 *
 * This function should be called from the init() method to create the data
 * blobs local to the pipeline.
 *
 * @param[in] type The type-name of the data blob to create.
 *
 * @return The method returns a pointer to the newly-created data blob.
 */
DataBlob* AbstractPipeline::createBlob(const QString& type)
{
    // Check that the blob factory exists.
    if (_blobFactory == NULL)
           throw QString("AbstractPipeline::createBlob(): No data blob factory.");

    DataBlob* blob = _blobFactory->create(type);
    return blob;
}

/**
 * @details
 * Creates a new module using the pipeline application's module factory.
 *
 * This function should be called from the init() method to create the modules
 * required by the pipeline.
 *
 * @param[in] type The type-name of the module to create.
 * @param[in] name The name of the module to create. Name is used in the XML
 *                 configuration to differentiate the use of several modules
 *                 of the same type within a pipeline.
 *
 * @return The method returns a pointer to the newly-created module.
 */
AbstractModule* AbstractPipeline::createModule(const QString& type,
        const QString& name)
{
    // Check that the module factory exists.
    if (_moduleFactory == NULL)
           throw QString("AbstractPipeline::createModule(): No module factory.");

    AbstractModule* module = _moduleFactory->create(type, name);
    module->setPipeline(this);
    _modules.append(module);
    return module;
}

const QList<DataBlob*>& AbstractPipeline::streamHistory(const QString& stream) const
{
      return *(_streamHistory[stream]);
}

ConfigNode AbstractPipeline::config( const QString& tag, const QString& name )
{
     return _pipelineDriver->config( tag, name );
}

void AbstractPipeline::copyConfig( AbstractPipeline* pipeline ) const
{
    pipeline->setBlobFactory(_blobFactory);
    pipeline->setModuleFactory(_moduleFactory);
    pipeline->setPipelineDriver(_pipelineDriver);
    pipeline->setOutputStreamManager(_osmanager);
}

void AbstractPipeline::exec( QHash<QString,DataBlob*>& data )
{
      // update the history information
      // ensure latest is at the front of the list
      foreach( const QString& stream, _streamHistory.keys() ) {
          if( data.contains(stream) ) {
              QList<DataBlob*>& h = *(_streamHistory[stream]);
              DataBlob* blob=data[stream];
#if QT_VERSION >= 0x040300
              h.removeOne(blob); 
#else
              h.removeAll(blob); 
#endif
              h.push_front(blob);
          }
      }
      run(data);
}

/**
 * @details
 * Requests remote data from the data client.
 */
void AbstractPipeline::requestRemoteData(const QString& type, 
                                         unsigned int history)
{
    _requiredDataRemote.addStreamData(type);
    _history[type]=history;
    if( ! _streamHistory.contains(type) ) {
        _streamHistory[type]=new QList<DataBlob*>;
    }
}

unsigned int AbstractPipeline::historySize(const QString& type) const {
    if( _requiredDataRemote.contains(type) ) {
        if( _history.contains(type) )
            return _history[type];
        return 1; // defaults to one (no history required)
    }
    return 0; // unknown stream
}

/**
 * @details
 * Returns all the remote data required by this pipeline.
 *
 * @return Returns the remote data required by the pipeline.
 */
const DataRequirements& AbstractPipeline::requiredDataRemote() const
{
    return _requiredDataRemote;
}

/**
 * @details
 * Sets the pointer to the data blob factory.
 *
 * @param[in] factory Pointer to the data blob factory to use.
 */
void AbstractPipeline::setBlobFactory(FactoryGeneric<DataBlob>* factory)
{
    _blobFactory = factory;
}

/**
 * @details
 * Sets the pointer to the module factory.
 *
 * @param[in] factory Pointer to the module factory to use.
 */
void AbstractPipeline::setModuleFactory(FactoryConfig<AbstractModule>* factory)
{
    _moduleFactory = factory;
}

/**
 * @details
 * This function is provided for the pipeline to disable itself
 * and so not be called by the pipeline driver
 */

void AbstractPipeline::deactivate()
{
    if (_pipelineDriver == NULL)
        throw QString("AbstractPipeline::disable(): No pipeline driver.");

    _pipelineDriver->deactivatePipeline(this);
}

/**
 * @details
 * Sends data to the output streams managed by the OutputStreamManger
 *
 * @param[in] DataBlob to be sent.
 * @param[in] name of the output stream (defaults to DataBlob->type()).
 */
void AbstractPipeline::dataOutput( const DataBlob* data, const QString& stream ) const
{
     _osmanager->send(data, stream);
}

/**
 * @details
 * Sets the pointer to the pipeline driver.
 *
 * @param[in] driver Pointer to the pipeline driver to use.
 */
void AbstractPipeline::setPipelineDriver(PipelineDriver* driver)
{
    _pipelineDriver = driver;
}

/**
 * @details
 * Sets the pointer to the output streamer to be used for dataOuput() calls.
 *
 * @param[in] driver Pointer to the Output Stream Manager.
 */
void AbstractPipeline::setOutputStreamManager(OutputStreamManager* osmanager)
{
    _osmanager = osmanager;
}

/**
 * @details
 * This protected function is provided for the pipeline to stop the
 * pipeline driver, and should only be called under abnormal termination
 * conditions.
 */
void AbstractPipeline::stop()
{
    if (_pipelineDriver == NULL)
        throw QString("AbstractPipeline::stop(): No pipeline driver.");

    _pipelineDriver->stop();
}

} // namespace pelican

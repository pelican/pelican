#include <QtCore/QVector>
#include <boost/shared_ptr.hpp>
#include "pelican/server/ChunkerManager.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/AbstractChunker.h"

#include "pelican/utility/memCheck.h"

// include default chunkers
// here to ensure they are available in the factory
#include "pelican/server/FileChunker.h"

namespace pelican {

/**
 * @details
 * Constructs the ChunkerManager.
 */
ChunkerManager::ChunkerManager(const Config* config, const QString& section)
{
    // Create the chunker factory.
    _factory = new FactoryConfig<AbstractChunker>(config, section, "chunkers");
}

ChunkerManager::ChunkerManager(const Config* config,  const Config::TreeAddress& base )
{
    // Create the chunker factory.
    Config::TreeAddress chunkerBase = base;
    chunkerBase << Config::NodeId("chunkers","");

    _factory = new FactoryConfig<AbstractChunker>(config, chunkerBase);
}
/**
 * @details
 * Destroys the ChunkerManager and the chunker factory it contains.
 */
ChunkerManager::~ChunkerManager()
{
    // Iterate over all data receivers and stop them.
    foreach (DataReceiver* receiver, _dataReceivers) {
        delete receiver;
    }

    // Delete the chunker factory.
    delete _factory;
}

/**
 * @details
 * Initialises the registered chunkers and calls the start() method on each
 * data receiver.
 */
void ChunkerManager::init(DataManager& dataManager)
{
    // Create the stream and service data buffers.
    foreach (const QString& type, _streamDataTypes) {
        dataManager.getStreamBuffer(type);
    }

    foreach (const QString& type, _serviceDataTypes) {
        dataManager.getServiceBuffer(type);
    }

    //QList<QPair<QString,quint16> > inputPorts = _chunkerPortMap.keys();

    // Set up data-stream inputs.
    //for (int i = 0; i < inputPorts.size(); ++i) {
    //    AbstractChunker* chunker = _chunkerPortMap[inputPorts[i]];
    //    chunker->setDataManager(&dataManager);
    //    DataReceiver* receiver = new DataReceiver(chunker);
    //    _dataReceivers.append(receiver);
    //    receiver->start();
    //}
    // Set up file watchers.
    foreach (AbstractChunker* chunker, _chunkers ) {
        chunker->setDataManager(&dataManager);
        DataReceiver* receiver = new DataReceiver(chunker);
        _dataReceivers.append(receiver);
        receiver->start();
    }
}

bool ChunkerManager::isRunning() const
{
    if( _dataReceivers.size() == 0 )
        return false;

    bool rv = true;
    foreach (DataReceiver* dr, _dataReceivers ) {
         rv &= dr->isRunning();
    }
    return rv;
}

/**
 * @details
 * Adds a stream chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
void ChunkerManager::addStreamChunker(QString type, QString name)
{
    Q_ASSERT(type != "" );
    AbstractChunker* chunker = _factory->create(type, name);
    if( chunker->type() == "" )
        chunker->setType(type);
    _addChunker(chunker);
    _streamDataTypes.insert(chunker->type());
}

/**
 * @details
 * Adds a service chunker of the given \p type and \p name.
 *
 * @param type The chunker type (class name).
 * @param name The optional chunker configuration name.
 */
void ChunkerManager::addServiceChunker(QString type, QString name)
{
    Q_ASSERT(type != "" );
    AbstractChunker* chunker = _factory->create(type, name);
    if( chunker->type() == "" )
        chunker->setType(type);
    _addChunker(chunker);
    _serviceDataTypes.insert(chunker->type());
}

/**
 * @details
 * Adds the allocated chunker to the map of known chunkers.
 *
 * @param[in] chunker  Pointer to the allocated chunker.
 */
void ChunkerManager::_addChunker(AbstractChunker* chunker)
{
    if (chunker) {
        //if (_streamDataTypes.contains(chunker->type()) ||
        //        _serviceDataTypes.contains(chunker->type())) {
        //    throw QString("ChunkerManager: Input stream '%1' "
        //            "is already assigned").arg(chunker->type());
        //}

        QPair<QString,quint16> pair(chunker->host(), chunker->port());
        // check to see multiple chunkers are not assigned to the
        // same host and port
        if (!chunker->host().isEmpty())
        {
            if (_chunkerPortMap.contains(pair)) {
                throw QString("ChunkerManager: Cannot map multiple chunkers "
                        "to the same host/port (%1/%2) - already assigned to stream %3").
                        arg(chunker->host()).arg(chunker->port()).arg(_chunkerPortMap[pair]->type());
            }
            _chunkerPortMap[pair] = chunker;
        }
        _chunkers.insert(chunker);
    }
}

} // namespace pelican

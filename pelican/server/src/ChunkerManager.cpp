#include <QtCore/QVector>
#include <boost/shared_ptr.hpp>
#include "pelican/server/ChunkerManager.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/AbstractChunker.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the ChunkerManager.
 */
ChunkerManager::ChunkerManager(const Config* config, const QString section)
{
    // Initialise members.
    _config = config;

    // Create the chunker factory.
    _factory = new FactoryConfig<AbstractChunker>(config, section, "chunkers");
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

    QList<QPair<QString,quint16> > inputPorts = _chunkerPortMap.keys();

    // Set up data-stream inputs.
    for (int i = 0; i < inputPorts.size(); ++i) {
        AbstractChunker* chunker = _chunkerPortMap[inputPorts[i]];
        chunker->setDataManager(&dataManager);
        DataReceiver* receiver = new DataReceiver(chunker);
        _dataReceivers.append(receiver);
        receiver->start();
    }
    // Set up file watchers.
    foreach (AbstractChunker* chunker, _watchingChunkers.values() ) {
        chunker->setDataManager(&dataManager);
        DataReceiver* receiver = new DataReceiver(chunker);
        _dataReceivers.append(receiver);
        receiver->start();
    }
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
    AbstractChunker* chunker = _factory->create(type, name);
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
    AbstractChunker* chunker = _factory->create(type, name);
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
        if (!chunker->watchFile().isEmpty())
        {
            QString file = chunker->watchFile();
            if (_watchingChunkers.contains(file)) {
                throw QString("ChunkerManager: Cannot map multiple chunkers "
                        "to the same file (%1)").arg(file);
            }
            _watchingChunkers[chunker->watchFile()] = chunker;
        }
        else if (!chunker->host().isEmpty())
        {
            if (_chunkerPortMap.contains(pair)) {
                throw QString("ChunkerManager: Cannot map multiple chunkers "
                        "to the same host/port (%1/%2)").
                        arg(chunker->host()).arg(chunker->port());
            }
            _chunkerPortMap[pair] = chunker;
        }
    }
}

} // namespace pelican

#include "pelican/server/ChunkerManager.h"
#include "pelican/server/DataReceiver.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/AbstractChunker.h"

// NOTE: Include default chunkers here to ensure they are available in the
// factory.
#include "pelican/server/FileChunker.h"

#include <QtCore/QVector>

#include <boost/shared_ptr.hpp>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the ChunkerManager, creating the chucker factory.
 */
ChunkerManager::ChunkerManager(const Config* config, const QString& section)
{
    _factory = new FactoryConfig<AbstractChunker>(config, section, "chunkers");
}


/**
 * @details
 * Constructs the ChunkerManager, creating the chucker factory.
 */
ChunkerManager::ChunkerManager(const Config* config,  const Config::TreeAddress& base )
{
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
bool ChunkerManager::init(DataManager& dataManager)
{
    // Create the stream and service data buffers.
    foreach (const QString& type, _streamDataTypes) {
        dataManager.getStreamBuffer(type);
    }

    foreach (const QString& type, _serviceDataTypes) {
        dataManager.getServiceBuffer(type);
    }

    try {
        foreach (AbstractChunker* chunker, _chunkers ) {
            chunker->setDataManager(&dataManager);
            DataReceiver* receiver = new DataReceiver(chunker);
            _dataReceivers.append(receiver);
            receiver->start();
        }
    }
    catch(const QString& msg)
    {
        cout << "Error Initiating Chunkers: " << msg.toStdString() << endl;
        return false;
    }
    return true;
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
void ChunkerManager::addStreamChunker(QString chunkerType, QString name)
{
    Q_ASSERT(chunkerType != "");

    // Create a chunker of the specified type with the factory.
    AbstractChunker* chunker = _factory->create(chunkerType, name);

    // If no data chunk types are registered to the chunker, set the chunker
    // data type to the chunker name.
    // NOTE: this should probably never happen!
    if (chunker->chunkTypes().empty())
    {
        cerr << "ChunkerManager::addStreamChunker(): WARNING Chunker '"
             << chunkerType.toStdString() << "' has no registered chunk data types."
             << endl;
        chunker->addChunkType(chunkerType);
    }

    // Add the chunker to the chunker manager.
    _addChunker(chunker);

    // Loop over the list of chunk types written into the data buffer
    // by the chunker and add them to the list of stream data types.
    foreach (QString chunkType, chunker->chunkTypes())
        _streamDataTypes.insert(chunkType);
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

    // Create a chunker of the specified type with the factory.
    AbstractChunker* chunker = _factory->create(type, name);

    // If no types are registered to the chunker set the chunker data type
    // to the chunker name.
    if (chunker->chunkTypes().empty())
    {
        cerr << "ChunkerManager::addStreamChunker(): WARNING Chunker '"
             << type.toStdString() << "' has no registered chunk data types."
             << endl;
        chunker->addChunkType(type);
    }

    // Add the chunker to the chunker manager.
    _addChunker(chunker);

    // Loop over the list of chunk types written into the data buffer
    // by the chunker and add them to the list of service data types.
    foreach (QString chunkType, chunker->chunkTypes())
        _serviceDataTypes.insert(chunkType);
}


/**
 * @details
 * Adds the allocated chunker to the map of known chunkers.
 *
 * @param[in] chunker  Pointer to the allocated chunker.
 */
void ChunkerManager::_addChunker(AbstractChunker* chunker)
{
    if (chunker)
    {
        QPair<QString, quint16> pair(chunker->host(), chunker->port());

        // Check to see any chunkers are already assigned to the same host
        // and port.
        if (!chunker->host().isEmpty())
        {
            if (_chunkerPortMap.contains(pair)) {
                QString err = "ChunkerManager::_addChunker(): ";
                err += "Cannot map multiple chunkers to the same host/ port. ";
                err += "(host %1, port %2 already assigned)";
                err = err.arg(chunker->host());
                err = err.arg(chunker->port());
                throw err;
            }
            _chunkerPortMap[pair] = chunker;
        }
        _chunkers.insert(chunker);
    }
}

} // namespace pelican

#include <QVector>
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
    _factory = new Factory<AbstractChunker>(config, section, "chunkers");
}

/**
 * @details
 * Destroys the ChunkerManager and the chunker factory it contains.
 */
ChunkerManager::~ChunkerManager()
{
    // Delete the chunker factory.
    delete _factory;
}

/**
 * @details
 * Initialises the registered chunkers and calls the listen() method on each
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
        boost::shared_ptr<DataReceiver> receiver( new DataReceiver(chunker, &dataManager) );
        _dataReceivers.append(receiver);
        receiver->listen();
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
 *
 */
void ChunkerManager::_addChunker(AbstractChunker* chunker)
{
    if( chunker ) {
        //if( _streamDataTypes.contains(chunker->type()) || _serviceDataTypes.contains(chunker->type()) ) {
        //    throw( QString("PelicanServer:  input stream \"") + chunker->type() + "\" is already assigned");
        //}
        QPair<QString,quint16> pair(chunker->host(), chunker->port());
        if (!chunker->host().isEmpty()) {
            if( _chunkerPortMap.contains(pair) ) {
                throw QString("Cannot map multiple chunkers to the same host/port ("
                        + chunker->host() + "/" + QString().setNum(chunker->port())) + ")";
            }
        }
        _chunkerPortMap[pair] = chunker;
    }
}

} // namespace pelican

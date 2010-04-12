#include <QVector>
#include <boost/shared_ptr.hpp>
#include "ChunkerManager.h"
#include "DataReceiver.h"
#include "DataManager.h"
#include "AbstractChunker.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the ChunkerManager.
 */
ChunkerManager::ChunkerManager()
{
}

/**
 * @details
 * Destroys the ChunkerManager, deleting all the chunkers it contains.
 */
ChunkerManager::~ChunkerManager()
{
    foreach (AbstractChunker* chunker, _chunkerPortMap) {
        delete chunker;
    }
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
        boost::shared_ptr<DataReceiver> receiver( new DataReceiver(chunker, &dataManager ) );
        _dataReceivers.append(receiver);
        receiver->listen();
    }
}

/**
 * @details
 */
void ChunkerManager::addStreamChunker(AbstractChunker* chunker)
{
    _addChunker(chunker);
    _streamDataTypes.insert( chunker->type() );
}

/**
 * @details
 *
 */
void ChunkerManager::addServiceChunker(AbstractChunker* chunker)
{
    _addChunker(chunker);
    _serviceDataTypes.insert( chunker->type() );
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
                delete chunker;
                throw QString("Cannot map multiple chunkers to the same host/port ("
                        + chunker->host() + "/" + QString().setNum(chunker->port())) + ")";
            }
        }
        _chunkerPortMap[pair] = chunker;
    }
}

} // namespace pelican

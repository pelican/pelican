#include <QVector>
#include <boost/shared_ptr.hpp>
#include "ChunkerManager.h"
#include "DataReceiver.h"
#include "DataManager.h"
#include "AbstractChunker.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details ChunkerManager 
 */
ChunkerManager::ChunkerManager( )
{
}

/**
 *@details
 */
ChunkerManager::~ChunkerManager()
{
    foreach( AbstractChunker* chunker, _chunkerPortMap)
    {
        delete chunker;
    }
}

void ChunkerManager::init(DataManager& dataManager )
{
//     QVector<boost::shared_ptr<DataReceiver> > dataReceivers;

    foreach( const QString& type, _streamDataTypes )
    {
        dataManager.getStreamBuffer(type); // creates the buffer
    }
    foreach( const QString& type, _serviceDataTypes )
    {
        dataManager.getServiceBuffer(type); // creates the buffer
    }

    QList<QPair<QString,quint16> > inputPorts = _chunkerPortMap.keys();

    // set up datastream inputs
    for (int i = 0; i < inputPorts.size(); ++i) {
        AbstractChunker* chunker = _chunkerPortMap[inputPorts[i]];
        chunker->setDataManager(&dataManager); // Added this.
        boost::shared_ptr<DataReceiver> receiver( new DataReceiver(chunker, &dataManager ) );
        _dataReceivers.append(receiver);
        receiver->listen();
    }
    std::cout << "Exit ChunkerManager::init()" << std::endl;
}

void ChunkerManager::addStreamChunker( AbstractChunker* chunker )
{
    _addChunker(chunker);
    _streamDataTypes.insert( chunker->type() );
}

void ChunkerManager::addServiceChunker( AbstractChunker* chunker )
{
    _addChunker(chunker);
    _serviceDataTypes.insert( chunker->type() );
}

void ChunkerManager::_addChunker( AbstractChunker* chunker)
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

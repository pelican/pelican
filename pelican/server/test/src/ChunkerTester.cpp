#include "ChunkerTester.h"
#include <QCoreApplication>
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/server/ChunkerManager.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details Constructs a ChunkerTester object.
 */
ChunkerTester::ChunkerTester( const QString& chunkerType, unsigned long bufferSize, const QString& XML_Config )
    : _chunkManager(0), _dataManager(0)
{
    _stream = chunkerType;
    Q_ASSERT( _stream != "" );
    Config::TreeAddress dataAddress, chunkerBase;
    chunkerBase << Config::NodeId("root", "");
    dataAddress << Config::NodeId("root", "") << Config::NodeId("data", "");
    QString xml = "<root>\n"
                  "<data>\n"
                      "<" + _stream + ">\n"
                          "<buffer maxSize=\"" + QString().setNum(bufferSize) + "\"/>\n"
                      "</" + _stream + ">\n"
                      "</data>\n"
                  "<chunkers>\n"
                      + XML_Config + "\n"
                  "</chunkers>\n"
                 "</root>\n";
    _config.setXML( xml );

    _chunkManager = new ChunkerManager( &_config, chunkerBase );
    _chunkManager->addStreamChunker( chunkerType );

    // setup the data manager
    _dataManager = new DataManager( &_config , dataAddress );


    // start the chunkers listening
    try {
        _chunkManager->init( *_dataManager );
        do {
            sleep(1); // at least one second seems to be necessary
        }
        while( ! _chunkManager->isRunning() );
    }
    catch( const QString& msg) 
    {
        std::cerr << "ChunkerTester:throw during construction: " << msg.toStdString() << std::endl;
        throw msg;
    }
}

/**
 * @details Destroys the ChunkerTester object.
 */
ChunkerTester::~ChunkerTester()
{
    delete _chunkManager;
    delete _dataManager;
}

AbstractChunker* ChunkerTester::chunker()
{
    QList<AbstractChunker* > chunkers = _chunkManager->chunkers().values();
    Q_ASSERT(chunkers.size() == 1 );
    Q_ASSERT(chunkers.first() != 0 );
    return chunkers.first();
}

/**
 * @details gets the next valid data from the queue
 */
LockedData ChunkerTester::getData()
{
    QCoreApplication::processEvents();
    return _dataManager->getNext(_stream);
}

int ChunkerTester::writeRequestCount() const
{
    QCoreApplication::processEvents();
    return _dataManager->getStreamBuffer(_stream)->numberOfActiveChunks();
}

} // namespace pelican

#include "ChunkerTester.h"
#include "pelican/server/StreamDataBuffer.h"
#include "pelican/server/ChunkerManager.h"

#include <QtCore/QCoreApplication>

#include <iostream>
using std::cerr;
using std::endl;

namespace pelican {
namespace test {

/**
 * @details Constructs a ChunkerTester object.
 */
ChunkerTester::ChunkerTester(const QString& chunkerType,
        unsigned long bufferSize, const QString& XML_Config, int verbose )
: _chunkManager(0), _dataManager(0)
{
    // Setup the XML configuration.
    Config::TreeAddress dataAddress, chunkerBase;
    chunkerBase << Config::NodeId("root", "");
    dataAddress << Config::NodeId("root", "") << Config::NodeId("data", "");
    QString bufsize = QString().setNum(bufferSize);
    QString xml = ""
            "<root>\n"
                "<data>\n";
         xml += "</data>\n"
            "<chunkers>\n"
                + XML_Config + "\n"
            "</chunkers>\n"
            "</root>\n";
    _config.setXML(xml);

    _chunkManager = new ChunkerManager(&_config, chunkerBase);
    AbstractChunker* chunker = _chunkManager->addStreamChunker(chunkerType);

    // setup the data manager
    _dataManager = new DataManager(&_config , dataAddress);
    _dataManager->setVerbosity(verbose);
    _streams = chunker->chunkTypes();
    foreach( const QString& type, chunker->chunkTypes() ) {
        _dataManager->setMaxBufferSize( type, bufferSize );
        _dataManager->setMaxChunkSize( type, bufferSize );
    }

    // start the chunkers listening
    try {
        _chunkManager->init(*_dataManager);
        do {
            sleep(1); // at least one second seems to be necessary
        }
        while(! _chunkManager->isRunning());
    }
    catch(const QString& msg)
    {
        cerr << "ChunkerTester:throw during construction: "
             << msg.toStdString() << endl;
        throw msg;
    }
}

/**
 * @details
 * Destroys the ChunkerTester object.
 */
ChunkerTester::~ChunkerTester()
{
    delete _chunkManager;
    delete _dataManager;
}


AbstractChunker* ChunkerTester::chunker() const
{
    QList<AbstractChunker* > chunkers = _chunkManager->chunkers().values();
    Q_ASSERT(chunkers.size() == 1);
    Q_ASSERT(chunkers.first() != 0);
    return chunkers.first();
}

QIODevice* ChunkerTester::getCurrentDevice() const {
    return _chunkManager->getCurrentDevice( chunker() );
}

/**
 * @details
 * Gets the next valid data from the queue.
 */
LockedData ChunkerTester::getData( const QString& stream )
{
    QString str = stream;
    if( str == "" ) { str = _streams[0]; }
    QCoreApplication::processEvents();
    return _dataManager->getNext(str);
}


int ChunkerTester::writeRequestCount(const QString& stream ) const
{
    QCoreApplication::processEvents();
    QString str = stream;
    if( str == "" ) { str = _streams[0]; }
    return _dataManager->getStreamBuffer(str)->numberOfActiveChunks();
}

} // namespace test
} // namespace pelican

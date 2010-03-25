#include "DataReceiver.h"
#include <QIODevice>
#include <QTimer>
#include <iostream>
#include "AbstractChunker.h"
#include "utility/pelicanTimer.h"


#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * DataReceiver constructor.
 */
DataReceiver::DataReceiver(AbstractChunker* chunker, DataManager* dm, QObject* parent)
    : QThread(parent), _chunker(chunker), _dm(dm)
{
    Q_ASSERT( dm != 0 );
    if ( !chunker ) throw QString("DataReceiver: Invalid Chunker");
    _device = 0;
    _port = 0;
}

/**
 * @details
 * DataReceiver destructor. This ensures that the thread has finished
 * executing (calling quit() if necessary) before returning.
 */
DataReceiver::~DataReceiver()
{
    // If the thread has started, call quit() to exit the event loop.
    if (isRunning()) while (!isFinished()) quit();
    wait();
    if( _device )
        delete _device;
}

/**
 * @details
 * Runs the thread owned by the DataReceiver object.
 * The thread creates and opens the socket and connects the readyRead()
 * socket signal to the processIncomingData() slot, before entering its
 * own event loop.
 *
 * The socket is destroyed when the event loop exits.
 */
void DataReceiver::run()
{
    // Open up the device to use.
    _device = _chunker->newDevice();
    std::cout << "DataReceiver::run" << std::endl;
    if( _device )
    {
        std::cout << "DataReceiver::run(): Socket OK" << std::endl;
        Q_ASSERT( connect(_device, SIGNAL( readyRead() ),
                    this, SLOT(processIncomingData()) ) );
        exec(); // Enter event loop here.
    }
    delete _device;
    _device = NULL; // Must be set to null here.
}

/**
 * @details
 * Starts the DataReceiver thread.
 */
void DataReceiver::listen()
{
//    _host = host;
//    if ( ! _host.isEmpty() && port > 0 ) {
//        _port = port;
        start();
//    }
}

/**
 * @details
 * This slot is called when data is available on the
 */
void DataReceiver::processIncomingData()
{
    std::cout << "Processing data in slot" << std::endl;
    _chunker->next( _device );
}

} // namespace pelican

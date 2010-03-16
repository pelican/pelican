#include "DataReceiver.h"
#include <QAbstractSocket>
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
    _socket = 0;
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
    if( _socket )
        delete _socket;
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
    // Open up a socket.
    _socket = _chunker->newSocket();
    if( _socket )
    {
        Q_ASSERT( connect(_socket, SIGNAL( readyRead() ),
                    this, SLOT(processIncomingData()) ) );
        _socket->connectToHost( _host, _port );
        exec(); // Enter event loop here.
    }
    delete _socket;
    _socket = NULL; // Must be set to null here.
}

/**
 * @details
 * Starts the DataReceiver thread if the host name and port number
 * have been set.
 *
 * @param[in] host A string containing the host name.
 * @param[in] port The port number to use.
 */
void DataReceiver::listen(const QString& host, quint16 port)
{
    _host = host;
    if ( ! _host.isEmpty() && port > 0 ) {
        _port = port;
        start();
    }
}

/**
 * @details
 * TODO Write a description of what this does, please!
 */
void DataReceiver::processIncomingData()
{
    _chunker->next( _socket );
}

} // namespace pelican

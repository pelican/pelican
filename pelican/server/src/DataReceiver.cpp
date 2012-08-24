#include "pelican/server/DataReceiver.h"
#include "pelican/utility/pelicanTimer.h"
#include <QtCore/QIODevice>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <iostream>

namespace pelican {

/**
 * @details
 * DataReceiver constructor.
 */
DataReceiver::DataReceiver(AbstractChunker* chunker) :
        QThread(), _chunker(chunker), _device(0)
{
    if (!chunker)
        throw QString("DataReceiver: Invalid chunker.");
}

/**
 * @details
 * DataReceiver destructor. This ensures that the thread has finished
 * executing (calling quit() if necessary) before returning.
 */
DataReceiver::~DataReceiver()
{
    _active = false; // mark destructor disconnect
    _chunker->stop();
    if( isRunning() ) {
        do quit(); 
        while( !wait(10) );// { terminate(); }
    }
    delete _device;
}

/**
 * @details
 * Runs the thread owned by the DataReceiver object.
 * The thread creates and opens the socket and connects the readyRead()
 * socket signal to the private _processIncomingData() slot using a direct
 * connection, before entering its own event loop.
 */
void DataReceiver::run()
{
    _active = true;
    // Open up the device to use.
    // N.B. must be done in this thread (i.e. in run())
    _setupDevice();
    //    int tid = syscall(__NR_gettid);
    //    std::cout << "Thread ID : " << tid << std::endl;
    if (_device) {
        // process any existing data on the stream
        if( _device->bytesAvailable() > 0 )
            _processIncomingData();
    }

    // enter main event loop (until quit() is called)
    exec();
}

void DataReceiver::_processIncomingData()
{
    _chunker->next(_device);
}

void DataReceiver::_registerSocket( QAbstractSocket* socket ) {
    connect( socket, SIGNAL( error(QAbstractSocket::SocketError) ), 
             SLOT(_processError())
             , Qt::DirectConnection );
    connect( socket, SIGNAL( disconnected() ), 
             SLOT(_reconnect())
             , Qt::DirectConnection);
}

void DataReceiver::_processError() {
    std::cerr << "DataReceiver: socket error: " << 
        _device->errorString().toStdString() << std::endl;
    _reconnect();
}

void DataReceiver::_reconnect() {
    _deleteDevice();
    if( ! _active ) _setupDevice();
}

void DataReceiver::_setupDevice() {
    _device = _chunker->newDevice();
    _chunker->activate();

    if( _device ) {
        // If its a socket we must try and catch the failure conditions
        connect(_device, SIGNAL(readyRead()), SLOT(_processIncomingData()),
                Qt::DirectConnection);
        if( QAbstractSocket* s = dynamic_cast<QAbstractSocket*>(_device) ) {
            _registerSocket( s );
        }
    }
}

void DataReceiver::_deleteDevice() {
    _chunker->stop();
    _device->disconnect();
    _device->deleteLater();
    _device = 0;
}

} // namespace pelican

#include "DataReceiver.h"
#include <QAbstractSocket>
#include "AbstractChunker.h"

#include "utility/memCheck.h"

namespace pelican {

// class DataReceiver 
DataReceiver::DataReceiver(AbstractChunker* chunker, DataManager* dm, QObject* parent)
    : QThread(parent), _chunker(chunker), _dm(dm)
{
    Q_ASSERT( dm != 0 );
    if( ! chunker ) throw QString("Invalid Chunker"); 
    _socket = 0;
    _port = 0;
}

DataReceiver::~DataReceiver()
{
    quit();
    if( _socket )
        delete _socket;
    wait();
}

void DataReceiver::run()
{
    // open up a socket
    _socket = _chunker->newSocket();
    if( _socket )
    {
        Q_ASSERT( connect(_socket, SIGNAL( readyRead() ),
                    this, SLOT(processIncomingData()) ) );
        _socket->connectToHost( _host, _port );
        exec();
    }
    delete _socket;
}

void DataReceiver::listen(const QString& host, quint16 port)
{
    _host = host;
    if( _host != "" && port > 0) {
        _port = port;
        start();
    }
}

void DataReceiver::processIncomingData()
{
    _chunker->next( _socket );
}

} // namespace pelican

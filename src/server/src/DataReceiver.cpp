#include "DataReceiver.h"
#include <QAbstractSocket>
#include <QTimer>
#include <iostream>
#include "AbstractChunker.h"
#include "utility/pelicanTimer.h"


#include "utility/memCheck.h"

namespace pelican {

// class DataReceiver
DataReceiver::DataReceiver(AbstractChunker* chunker, DataManager* dm, QObject* parent)
    : QThread(parent), _chunker(chunker), _dm(dm)
{
    Q_ASSERT( dm != 0 );
    if( ! chunker ) throw QString("Invalid Chunker");
//    connect(this, SIGNAL(started()), this, SLOT(die()), Qt::DirectConnection);
    _socket = 0;
    _port = 0;
}

DataReceiver::~DataReceiver()
{
    std::cout << "DARRGH!\n";
    quit();
    wait();
    if( _socket )
        delete _socket;
}

void DataReceiver::run()
{
    QTimer::singleShot(1, this, SLOT(die()));
    std::cout << "Running\n";
    TIMER_START
    exec();
    TIMER_STOP("done")
    std::cout << "After exec\n";



    // open up a socket
//    _socket = _chunker->newSocket();
//    if( _socket )
//    {
//        Q_ASSERT( connect(_socket, SIGNAL( readyRead() ),
//                    this, SLOT(processIncomingData()) ) );
//        _socket->connectToHost( _host, _port );
//        std::cout << "hello" << std::endl;
//        exec();
//        std::cout << "hello again" << std::endl;
//    }
//    delete _socket;
}

void DataReceiver::listen(const QString& host, quint16 port)
{
//    _host = host;
//    if( _host != "" && port > 0) {
//        _port = port;
//        std::cout <<"starting..." << std::endl;
        start();
//        std::cout <<"done..." << std::endl;
//    }
}

void DataReceiver::die()
{
    std::cout <<"hereA" << std::endl;
    quit();
    std::cout <<"hereB" << std::endl;
//    wait();
    std::cout <<"hereC" << std::endl;
}

void DataReceiver::processIncomingData()
{
    _chunker->next( _socket );
}

} // namespace pelican

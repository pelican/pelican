#include "DataReceiver.h"
#include <QAbstractSocket>
#include "AbstractChunker.h"


#include "utility/memCheck.h"

namespace pelican {


// class DataReceiver 
DataReceiver::DataReceiver(AbstractChunker* chunker, DataManager* dm, QObject* parent)
    : QObject(parent), _chunker(chunker), _dm(dm)
{
    _socket = _chunker->newSocket();
}

DataReceiver::~DataReceiver()
{
    delete _socket;
}

void DataReceiver::listen(const QString& host, quint16 port)
{
     _socket->connectToHost ( host, port );
     Q_ASSERT( connect(_socket, SIGNAL(readyRead()),
                          this, SLOT(processIncommingData()) ) );

}

void DataReceiver::processIncomingData()
{
    _chunker->next(_socket);
}

} // namespace pelican

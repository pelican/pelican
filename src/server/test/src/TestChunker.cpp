#include "TestChunker.h"
#include <QCoreApplication>
#include <QBuffer>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

QIODevice* TestChunker::newDevice()
{
    if(_badSocket)
        return 0;

//    QUdpSocket* socket = new QUdpSocket;
//    _device = socket;
//    socket->connectToHost( _host, _port );
//    return socket;

    QBuffer* buffer = new QBuffer;
    _device = buffer;
    return buffer;
}

/**
 * @details
 * Gets the next chunk of data from the UDP socket (if it exists).
 */
void TestChunker::next(QIODevice*)
{
    std::cout << "TestChunker::next" << std::endl;
    ++_nextCount;
    WritableData writableData = getDataStorage(_size);
//    writableData.write(); // TODO write the real data in here!
}

unsigned int TestChunker::nextCalled()
{
    unsigned int n = _nextCount;
    _nextCount = 0;
    return n;
}

} // namespace pelican

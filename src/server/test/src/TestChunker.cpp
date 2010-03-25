#include "TestChunker.h"
#include <QCoreApplication>
#include <QBuffer>
#include <QTimer>

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

    QTimer::singleShot(1000, this, SLOT(_start()));
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
    unsigned nDoubles = _size / sizeof(double);
    std::vector<double> array(nDoubles);
    for (unsigned i = 0; i < nDoubles; i++) {
        array[i] = i;
    }
    // Write some test data.
    writableData.write((void*)&array[0], _size, 0);
}

unsigned int TestChunker::nextCalled()
{
    unsigned int n = _nextCount;
    _nextCount = 0;
    return n;
}

/**
 * @details
 * Private slot to start the data stream.
 */
void TestChunker::_start()
{
    next(_device);
}

} // namespace pelican

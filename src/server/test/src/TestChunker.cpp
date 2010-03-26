#include "TestChunker.h"
#include <QCoreApplication>
#include <QBuffer>
#include <QTimer>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new TestChunker.
 */
TestChunker::TestChunker(const QString& type, bool badSocket, size_t size,
        QString host, quint16 port, QObject* parent)
: QObject(parent), AbstractChunker(type, host, port), _badSocket(badSocket),
_nextCount(0), _size(size)
{
    _timer = new QTimer;
    connect(_timer, SIGNAL(timeout()), this, SLOT(_start()));
}

/**
 * @details
 * Constructs a new device.
 */
QIODevice* TestChunker::newDevice()
{
    if(_badSocket)
        return 0;

//    QUdpSocket* socket = new QUdpSocket;
//    _device = socket;
//    socket->connectToHost( _host, _port );
//    return socket;

    _timer->start(1);
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
    std::cout << "TestChunker::next()" << std::endl;
    ++_nextCount;
    WritableData writableData = getDataStorage(_size);


    unsigned nDoubles = _size / sizeof(double);
    std::vector<double> array(nDoubles);
    for (unsigned i = 0; i < nDoubles; i++) {
//        array[i] = i;
        array[i] = _nextCount;
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
 * Private slot to start and add to the data stream.
 */
void TestChunker::_start()
{
    if (_nextCount < 10)
        next(_device);
    else
        _timer->stop();
}

} // namespace pelican

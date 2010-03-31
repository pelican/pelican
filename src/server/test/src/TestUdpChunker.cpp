#include "TestUdpChunker.h"

#include <QCoreApplication>
#include <QBuffer>
#include <QTimer>
#include <QUdpSocket>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new TestUdpChunker.
 */
TestUdpChunker::TestUdpChunker(const QString& type, QString host, quint16 port,
                               const size_t size)
        : AbstractChunker(type, host, port)
{
    _size = size;
    _nextCount = 0;
    _tempBuffer.resize(_size);
}


/**
 * @details
 * Destroys the TestUdpChunker.
 */
TestUdpChunker::~TestUdpChunker()
{
}


/**
 * @details
 * Constructs a new device from a QUdpSocket and opens it for reading.
 */
QIODevice* TestUdpChunker::newDevice()
{
    QUdpSocket* socket = new QUdpSocket;
    _device = socket;
    socket->bind(QHostAddress(host()), port());
    return socket;
}


/**
 * @details
 * Gets the next chunk of data from the UDP socket
 */
void TestUdpChunker::next(QIODevice* socket)
{
//     std::cout << "TestUdpChunker::next() size = " << _size << std::endl;

    qint64 sizeRead = static_cast<QUdpSocket*>(socket)->readDatagram(_tempBuffer.data(), _size);

//     double value = *reinterpret_cast<double*>(_tempBuffer.data());
//     std::cout << value << std::endl;
    
    if (sizeRead != _size)
        throw QString("TestUdpChunker::next(): size mismatch.");

    // Get writable data object.
    WritableData writableData = getDataStorage(_size);

    // If the writable data object is not valid, then return.
    if (!writableData.isValid())
        return;

    writableData.write((void*)_tempBuffer.data(), _size, 0);

    ++_nextCount;
}



} // namespace pelican

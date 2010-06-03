#include "pelican/server/test/TestUdpChunker.h"
#include "pelican/utility/Config.h"

#include <QtNetwork/QUdpSocket>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

// Register the chunker with the factory.
PELICAN_DECLARE_CHUNKER(TestUdpChunker)

/**
 * @details
 * Constructs a new TestUdpChunker.
 */
TestUdpChunker::TestUdpChunker(const ConfigNode& config)
    : AbstractChunker(config)
{
    // Check configuration node is correct.
    if (config.type() != "TestUdpChunker")
        throw QString("TestUdpChunker: Wrong configuration node.");

    // Get configuration options.
    _chunkSize = config.getOption("data", "chunkSize").toUInt();

    // Some sanity checking.
    if (type().isEmpty())
        throw QString("TestUdpChunker: Data type unspecified.");

    if (_chunkSize == 0)
        throw QString("TestUdpChunker: Chunk size zero.");

    // Initialise temporary storage and chunk counter.
    _nextCount = 0;
    _tempBuffer.resize(_chunkSize);
}


/**
 * @details
 * Constructs a new device from a QUdpSocket and opens it for reading.
 */
QIODevice* TestUdpChunker::newDevice()
{
    QUdpSocket* socket = new QUdpSocket;
    socket->bind(QHostAddress(host()), port());
    while (socket->state() != QUdpSocket::BoundState) {}
    return socket;
}

/**
 * @details
 * Gets the next chunk of data from the UDP socket
 */
void TestUdpChunker::next(QIODevice* device)
{
//    std::cout << "TestUdpChunker::next()" << std::endl;
    // Read the data off the UDP socket.
    QUdpSocket* socket = static_cast<QUdpSocket*>(device);
    while (!socket->hasPendingDatagrams()) {}
    qint64 sizeRead = socket->readDatagram(_tempBuffer.data(), _chunkSize);

    // Sanity check.
    if ((size_t)sizeRead != _chunkSize)
        throw QString("TestUdpChunker::next(): Size mismatch, sizeRead %1 != chunkSize %2.").arg(sizeRead).arg(_chunkSize);

    // Get writable data object.
    WritableData writableData = getDataStorage(_chunkSize);

    // If the writable data object is not valid, then return.
    if (!writableData.isValid())
        return;

    // Write the data into the buffer.
    writableData.write((void*)_tempBuffer.data(), _chunkSize, 0);
    ++_nextCount;
}

} // namespace pelican

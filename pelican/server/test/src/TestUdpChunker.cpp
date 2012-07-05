#include "pelican/server/test/TestUdpChunker.h"
#include "pelican/utility/Config.h"

#include <QtNetwork/QUdpSocket>
#include <iostream>

namespace pelican {
namespace test {

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
    _chunkSize = config.getOption("data", "chunkSize").toInt();

    // Some sanity checking.
    if (chunkTypes().empty())
        throw QString("TestUdpChunker: No chunk types registered to the chunker.");

    if (_chunkSize == 0)
        throw QString("TestUdpChunker: Chunk size zero.");
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
    // Get a pointer to the UDP socket.
    QUdpSocket* udpSocket = static_cast<QUdpSocket*>(device);
    _bytesRead = 0;

    // Get writable buffer space for the chunk.
    WritableData writableData = getDataStorage(_chunkSize);
    if (writableData.isValid()) {
        // Get pointer to start of writable memory.
        char* ptr = (char*) (writableData.ptr());

        // Read datagrams for chunk from the UDP socket.
        while (isActive() && _bytesRead < _chunkSize) {
            // Read the datagram, but avoid using pendingDatagramSize().
            bool ok = udpSocket->hasPendingDatagrams();
            if (!ok) {
                // MUST WAIT for the next datagram.
                if( ! isActive() ) return;
                udpSocket->waitForReadyRead(100);
                continue;
            }
            qint64 maxlen = _chunkSize - _bytesRead;
            qint64 length = udpSocket->readDatagram(ptr + _bytesRead, maxlen);
            if (length > 0)
                _bytesRead += length;
        }
    }

    // Must discard the datagram if there is no available space.
    else {
        if( ! isActive() ) return;
        udpSocket->readDatagram(0, 0);
    }
}

} // namespace test
} // namespace pelican

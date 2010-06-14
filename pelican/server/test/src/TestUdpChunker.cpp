#include "pelican/server/test/TestUdpChunker.h"
#include "pelican/utility/Config.h"

#include <QtNetwork/QUdpSocket>
//#include <sys/socket.h>
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
    _chunkSize = config.getOption("data", "chunkSize").toInt();

    // Some sanity checking.
    if (type().isEmpty())
        throw QString("TestUdpChunker: Data type unspecified.");

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

//    int v = 5000000;
//    if (::setsockopt(socket->socketDescriptor(), SOL_SOCKET, SO_RCVBUF, (char*) &v, sizeof(v)) == -1) {
//        std::cout << "Error!" << std::endl;
//    }

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
        udpSocket->readDatagram(0, 0);
    }
}

} // namespace pelican

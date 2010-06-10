#include "ChunkerExample.h"
#include "pelican/utility/Config.h"
#include <QtNetwork/QUdpSocket>

// Register the chunker with the factory.
PELICAN_DECLARE_CHUNKER(ChunkerExample)

// Construct the example chunker.
ChunkerExample::ChunkerExample(const ConfigNode& config)
    : AbstractChunker(config)
{
    // Set chunk size from the configuration.
    // The host, port and data type are set in the base class.
    _chunkSize = config.getOption("data", "chunkSize").toInt();
}

// Creates a suitable device ready for reading.
QIODevice* ChunkerExample::newDevice()
{
    // Return an opened QUdpSocket.
    QUdpSocket* socket = new QUdpSocket;
    socket->bind(QHostAddress(host()), port());

    // Wait for the socket to bind.
    while (socket->state() != QUdpSocket::BoundState) {}
    return socket;
}

// Called whenever there is data available on the device.
void ChunkerExample::next(QIODevice* device)
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
            // Read the datagram.
            qint64 length = udpSocket->pendingDatagramSize();
            if (length < 0) {
                // MUST WAIT for the next datagram.
                udpSocket->waitForReadyRead(100);
                continue;
            }
            if (_bytesRead + length <= _chunkSize)
                udpSocket->readDatagram(ptr + _bytesRead, length);
            _bytesRead += length;
        }
    }

    // Must discard the datagram if there is no available space.
    else {
        udpSocket->readDatagram(0, 0);
    }
}

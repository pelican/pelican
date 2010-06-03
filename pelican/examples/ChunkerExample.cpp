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
    chunkSize = config.getOption("data", "chunkSize").toInt();
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
    // Read all pending datagrams off the UDP socket.
    QUdpSocket* udpSocket = static_cast<QUdpSocket*>(device);
    while (udpSocket->hasPendingDatagrams()) {
        // Ensure enough space in the local buffer.
        qint64 length = udpSocket->pendingDatagramSize();
        int oldSize = buffer.size();
        buffer.resize(oldSize + length);

        // Read the datagram.
        udpSocket->readDatagram(buffer.data() + oldSize, length);
    }

    // Check if the local buffer contains enough data for a chunk.
    if (buffer.size() >= chunkSize) {
        // Get some writable buffer space.
        WritableData writableData = getDataStorage(chunkSize);

        if (writableData.isValid()) {
            // Write the chunk into the data manager.
            writableData.write((void*)buffer.constData(), chunkSize, 0);
        }

        // Compact the local buffer.
        buffer.remove(0, chunkSize);
    }
}

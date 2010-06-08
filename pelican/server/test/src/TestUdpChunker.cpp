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
    _chunkSize = config.getOption("data", "chunkSize").toInt();

    // Some sanity checking.
    if (type().isEmpty())
        throw QString("TestUdpChunker: Data type unspecified.");

    if (_chunkSize == 0)
        throw QString("TestUdpChunker: Chunk size zero.");

    // Initialise buffer.
    //_writableData = 0;
    _bytesRead = 0;
    _buffer.resize(_chunkSize);
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
//    // Read all pending datagrams off the UDP socket.
//    QUdpSocket* udpSocket = static_cast<QUdpSocket*>(device);
//    while (udpSocket->hasPendingDatagrams() && _buffer.size() < _chunkSize) {
//        // Ensure enough space in the local buffer.
//        qint64 length = udpSocket->pendingDatagramSize();
//        int oldSize = _buffer.size();
//        _buffer.resize(oldSize + length);
//
//        // Read the datagram.
//        udpSocket->readDatagram(_buffer.data() + oldSize, length);
//    }
//
//    // Check if the local buffer contains enough data for a chunk.
//    if (_buffer.size() >= _chunkSize) {
//        // Get some writable buffer space.
//        WritableData writableData = getDataStorage(_chunkSize);
//
//        if (writableData.isValid()) {
//            // Write the chunk into the data manager.
//            writableData.write((void*)_buffer.constData(), _chunkSize, 0);
//        }
//
//        // Compact the local buffer.
//        _buffer.remove(0, _chunkSize);
//    }

    ///////////////////////////////////////////////////////////

    // Get writable buffer space for the chunk, if required.
    WritableData writableData = getDataStorage(_chunkSize);

    QUdpSocket* udpSocket = static_cast<QUdpSocket*>(device);

    // If the writable data object is not valid, then discard the data.
    if (writableData.isValid()) {
        // Get pointer to start of writable memory.
        char* ptr = (char*)(writableData.ptr());

        // Read all pending datagrams off the UDP socket.
        while (_bytesRead < _chunkSize) {
            // Read the datagram.
            qint64 length = udpSocket->pendingDatagramSize();
            udpSocket->readDatagram(ptr + _bytesRead, length);
            _bytesRead += length;
        }

        // Check if we have received enough for a chunk.
        if (_bytesRead >= _chunkSize) _bytesRead = 0;
    }
    else {
        udpSocket->readDatagram(0, 0);
    }

    ///////////////////////////////////////////////////////////

    // Read the data off the UDP socket.
    QUdpSocket* socket = static_cast<QUdpSocket*>(device);
    qint64 sizeRead = socket->readDatagram(_buffer.data(), _chunkSize);

    // Sanity check.
    if (sizeRead != _chunkSize)
        throw QString("TestUdpChunker::next(): Size mismatch, sizeRead %1 != chunkSize %2.").arg(sizeRead).arg(_chunkSize);

    // Get writable data object.
    WritableData writableData = getDataStorage(_chunkSize);

    // If the writable data object is not valid, then return.
    if (!writableData.isValid())
        return;

    // Write the data into the buffer.
    writableData.write((void*)_buffer.data(), _chunkSize, 0);
    //writableData.write((void*)_buffer.data(), 1, 0);
}

} // namespace pelican

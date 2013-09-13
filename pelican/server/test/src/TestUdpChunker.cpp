#include "server/test/TestUdpChunker.h"
#include "utility/Config.h"

#include <QtNetwork/QUdpSocket>
#include <iostream>

namespace pelican {
namespace test {

/**
 * @details
 * Constructs a new TestUdpChunker.
 */
TestUdpChunker::TestUdpChunker(const ConfigNode& config)
    : AbstractChunker(config), _bytesRead(0), _chunkCounter(0)
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
#if 0
    using namespace std;
    cout << "TestUdpChunker::next() " << name().toStdString() << endl;
#endif

    // Get a pointer to the UDP socket.
    QUdpSocket* udpSocket = static_cast<QUdpSocket*>(device);
    _bytesRead = 0;

//    cout << "chunk counter = " << _chunkCounter << endl;

    // Get writable buffer space for the chunk.
    WritableData writableData = getDataStorage(_chunkSize);

    // Must discard the datagram if its not possible to
    // obtain a valid chunk to write into.
    if (!writableData.isValid()) {
//        cerr << "-- Invalid chunk ...!" << endl;
//        std::string active = (isActive() ? "true" : "false");
//        cerr << "-- isActive? " << active << endl;
//
//        cerr << "-- # usable chunks = " << numUsableChunks(_chunkSize) << endl;
//        cerr << "-- max size = " << maxBufferSize() << endl;
//        cerr << "-- max chunk size = " << maxChunkSize() << endl;
//        cerr << "-- allocated = " << allocatedSize() << endl;
        if (!isActive())
            return;
        udpSocket->readDatagram(0, 0);
        return;
    }

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

    ++_chunkCounter;

#if 0
    cout << "TestUdpChunker::next() READ CHUNK! "
            << name().toStdString()
            << " : " << _chunkCounter
            << endl;
#endif
}

} // namespace test
} // namespace pelican

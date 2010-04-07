#include "TestUdpChunker.h"

#include <QCoreApplication>
#include <QBuffer>
#include <QTimer>
#include <QUdpSocket>

#include <iostream>

#include "utility/memCheck.h"
#include <utility/Config.h>

namespace pelican {


    /**
* @details
* Constructs a new TestUdpChunker.
*/
TestUdpChunker::TestUdpChunker(const ConfigNode& config)
: AbstractChunker() 
{
    if (config.type() != "TestUdpChunker")
        throw QString("TestUdpChunker::TestUdpChunker(): Wrong config.");

    // Get configuration options.
    setHost(config.getOption("connection", "host"));
    setPort(qint16(config.getOption("connection", "port").toUInt()));
    setType(config.getOption("data", "type"));
    _chunkSize = config.getOption("data", "chunkSize").toUInt();

    // Some sanity checking.
    if (type().isEmpty())
        throw QString("TestUdpChunker::TestUdpChunker(): Data type unspecified.");
    
    if (_chunkSize == 0)
        throw QString("TestUdpChunker::TestUdpChunker(): Chunk size zero.");

    // Initialise temporary storage and chunk counter.
    _nextCount = 0;
    _tempBuffer.resize(_chunkSize);
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

    qint64 sizeRead = static_cast<QUdpSocket*>(socket)->readDatagram(_tempBuffer.data(), _chunkSize);

//     double value = *reinterpret_cast<double*>(_tempBuffer.data());
//     std::cout << value << std::endl;
    
    if (sizeRead != _chunkSize)
        throw QString("TestUdpChunker::next(): size mismatch.");

    // Get writable data object.
    WritableData writableData = getDataStorage(_chunkSize);

    // If the writable data object is not valid, then return.
    if (!writableData.isValid())
        return;

    writableData.write((void*)_tempBuffer.data(), _chunkSize, 0);

    ++_nextCount;
}



} // namespace pelican

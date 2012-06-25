#include "pelican/server/test/TestChunker.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QBuffer>

#include <iostream>

namespace pelican {
namespace test {

/**
 * @details
 * Constructs a new TestChunker.
 */
TestChunker::TestChunker(const QString& type, bool badSocket, size_t size,
        const QString& host, quint16 port, QObject* parent)
: QThread(parent), AbstractChunker()
{
    addChunkType(type);
    setHost(host);
    setPort(port);
    _device = 0;
    _badSocket = badSocket;
    _size = size;
    start();
}

/**
 * @details
 * Constructs a new TestChunker.
 */
TestChunker::TestChunker(const ConfigNode& config)
: QThread(), AbstractChunker(config)
{
    _device = 0;
    _badSocket = false;
    _size = config.getOption("data", "chunkSize", "512").toUInt();
    start();
}

/**
 * @details
 * Destroys the TestChunker.
 */
TestChunker::~TestChunker()
{
    _abort = true;
    wait();
}

/**
 * @details
 * Constructs a new device.
 */
QIODevice* TestChunker::newDevice()
{
    if (_badSocket)
        return 0;

    if (!_device) {
        _device = new QBuffer;
    }
    return _device;
}

/**
 * @details
 * Gets the next chunk of data from the device (if it exists).
 */
void TestChunker::next(QIODevice* device)
{
    // Read data off the device.
    device->open(QIODevice::ReadOnly);
    QByteArray array = device->readAll();
    device->close();
    if (array.size() == 0) {
        std::cout << "Nothing to read!" << std::endl;
        return;
    }

    // Get writable data object.
    WritableData writableData(0);
    try {
        writableData = getDataStorage(_size);
    }
    catch (QString e) {
        std::cout << "Unexpected exception: " << e.toStdString() << std::endl;
        return;
    }

    // If the writable data object is not valid, then return.
    if (!writableData.isValid())
        return;

    // ... or, block until writable data is valid.
    while (!writableData.isValid()) {
        writableData = getDataStorage(_size);
        usleep(1);
    }

    // Write some test data.
    writableData.write((void*)array.data(), _size, 0);
}

/**
 * @details
 * Runs the thread owned by the test chunker.
 */
void TestChunker::run()
{
    msleep(50);

    // Run the thread.
    unsigned counter = 0;
    unsigned nDoubles = _size / sizeof(double);
    std::vector<double> array(nDoubles);

    while (!_abort) {
        if (_device) {
            for (unsigned i = 0; i < nDoubles; i++)
                array[i] = counter;
            counter++;
            _device->open(QIODevice::WriteOnly);
            _device->write(reinterpret_cast<char*>(&array[0]), _size);
            _device->close();
        }

        msleep(1);
    }
}

} // namespace test
} // namespace pelican

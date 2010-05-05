#include "pelican/server/test/TestChunker.h"

#include <QCoreApplication>
#include <QBuffer>

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_CHUNKER(TestChunker)

/**
 * @details
 * Constructs a new TestChunker.
 */
TestChunker::TestChunker(const QString& type, bool badSocket, size_t size,
        QString host, quint16 port, QObject* parent) :
            QThread(parent), AbstractChunker(type, host, port)
{
    _device = 0;
    _nextCount = 0;
    _badSocket = badSocket;
    _size = size;
    start();
}

/**
* @details
* Constructs a new TestChunker.
*/
TestChunker::TestChunker(const ConfigNode& config) : QThread(),
        AbstractChunker(config)
{
    _device = 0;
    _nextCount = 0;
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

    if (_device) {
        _device = new QBuffer;
        _device->open(QIODevice::ReadWrite);
        setDevice(_device);
    }
    return _device;
}

/**
 * @details
 * Gets the next chunk of data from the device (if it exists).
 */
void TestChunker::next(QIODevice*)
{
    //std::cout << "TestChunker::next()" << std::endl;
    ++_nextCount;

    // Get writable data object.
    WritableData writableData(0);
//    std::cout << "Next count " << _nextCount << std::endl;
    try {
        writableData = getDataStorage(_size);
    } catch (QString e) {
        std::cout << "Unexpected exception: " << e.toStdString() << std::endl;
    }

    // If the writable data object is not valid, then return.
    if (!writableData.isValid())
        return;

    // ... or, block until writable data is valid.
    while (!writableData.isValid()) {
        writableData = getDataStorage(_size);
        usleep(1);
    }

    unsigned nDoubles = _size / sizeof(double);
    std::vector<double> array(nDoubles);
    for (unsigned i = 0; i < nDoubles; i++) {
//        array[i] = i;
        array[i] = _nextCount;
    }

    // Write some test data.
    writableData.write((void*)&array[0], _size, 0);
}

unsigned int TestChunker::nextCalled()
{
    unsigned int n = _nextCount;
    _nextCount = 0;
    return n;
}

/**
 * @details
 * Runs the thread owned by the test chunker.
 */
void TestChunker::run()
{
    msleep(50);

    // Call next().
    while (!_abort) {
        next(_device);
        msleep(10);
    }
}

} // namespace pelican

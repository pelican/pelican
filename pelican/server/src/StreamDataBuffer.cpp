#include "pelican/server/StreamDataBuffer.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/WritableData.h"
#include "pelican/comms/StreamData.h"

#include <QtCore/QMutexLocker>
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
#include <complex>

#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QIODevice>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the stream data buffer.
 *
 * @param type         A string containing the type of data held in the buffer.
 * @param max          The maximum size of the buffer in bytes.
 * @param maxChunkSize The maximum chunk size in bytes.
 * @param parent       (Optional.) Pointer to the object's parent.
 */
StreamDataBuffer::StreamDataBuffer(const QString& type,
        const size_t max, const size_t maxChunkSize, QObject* parent) :
        AbstractDataBuffer(type, parent)
{
    _max = max;
    _maxChunkSize = maxChunkSize;
    _space = _max; // Buffer initially empty so space = max size.
    _manager = 0;
    _counter = 0;
}


/**
 * @details
 * Destroys the stream data buffer, freeing the memory held by it.
 */
StreamDataBuffer::~StreamDataBuffer()
{
    foreach (LockableStreamData* data, _data) {
        free(data->data()->data());
        delete data;
    }
}


/**
 * @details
 * Gets the next block of data to serve.
 */
void StreamDataBuffer::getNext(LockedData& lockedData)
{
    QMutexLocker locker(&_mutex);

    // Check if the serve queue is empty.
    if (_serveQueue.isEmpty()) {
        lockedData.setData(0); // Return an invalid data block.
        return;
    }

    // Remove the data from the serve queue.
    lockedData.setData(_serveQueue.dequeue());
}


/**
 * @details
 * Gets a writable data object of the given size and returns it.
 *
 * @param[in] size The size of the writable data to return.
 */
WritableData StreamDataBuffer::getWritable(size_t size)
{
    QMutexLocker locker(&_writeMutex);
    LockableStreamData* lockableStreamData = _getWritable(size);

    // Prepare the object for use by adding Service Data info.
    if (lockableStreamData) {
        lockableStreamData->reset();
        if (!_manager)
            throw QString("StreamDataBuffer::getWritable(): No data manager.");
        _manager->associateServiceData(lockableStreamData);
    }

    return WritableData(lockableStreamData);
}


/**
 * @details
 * Private method to get a writable block of memory of the given size.
 *
 * @return Returns a pointer to the LockableStreamData object to use.
 *
 * @param[in] size The size in bytes of the requested block of writable memory.
 */
LockableStreamData* StreamDataBuffer::_getWritable(size_t size)
{
    // Return a pre-allocated block from the empty queue, if one exists.
    for (int i = 0; i < _emptyQueue.size(); ++i) {
        LockableStreamData* lockableData = _emptyQueue[i];

        if( lockableData->maxSize() >= size ) {
            // We found one, so our work is done.
            _emptyQueue.removeAt(i);
            return lockableData;
        }
    }

    // There are no empty containers already available, so we
    // create a new data object if we have enough space.
    if (size <= _space && size <= _maxChunkSize)
    {
        void* memory = calloc(size, sizeof(char)); // Released in destructor.
        if (memory) {
            _space -= size;
            LockableStreamData* lockableData = new LockableStreamData(_type, memory, size);
            _data.append(lockableData); // Add to the list of known data.
            connect(lockableData, SIGNAL(unlockedWrite()), SLOT(activateData()));
            connect(lockableData, SIGNAL(unlocked()), SLOT(deactivateData()));
            return lockableData;
        }
    }

    // No free containers and no space left, so we return an invalid pointer.
    return 0;
}


/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlockedWrite() signal. It calls the method to activate the data
 * chunk that emitted the signal by putting it onto the serve queue.
 */
void StreamDataBuffer::activateData()
{
    activateData(static_cast<LockableStreamData*>(sender()));
}


/**
 * @details
 * This protected slot is called when the lockable data object
 * emits the unlocked() signal. It calls the method to deactivate the data
 * chunk that emitted the signal by putting it onto the empty queue.
 */
void StreamDataBuffer::deactivateData()
{
    deactivateData(static_cast<LockableStreamData*>(sender()));
}


/**
 * @details
 * Puts the specified chunk of data on the empty queue, which allows the
 * space it occupied to be reused.
 */
void StreamDataBuffer::deactivateData(LockableStreamData* data)
{
    QMutexLocker locker(&_mutex);
    if (!data->served()) {
        _serveQueue.prepend(data);
        return;
    }
    data->reset();

    QMutexLocker writeLocker(&_writeMutex);
    _emptyQueue.push_back(data);
}


/**
 * @details
 * Puts the specified chunk of data on the queue ready to be served.
 */
void StreamDataBuffer::activateData(LockableStreamData* data)
{
    if (data->isValid()) {
        QMutexLocker locker(&_mutex);

        StreamData* s = data->streamData();
//        cout << "StreamDataBuffer::activateData size = " << s->size() << endl;

        typedef std::complex<short> i16c;
        if (s->name() == "LofarTimeStream1")
        {
            //cout << "stream name = " << s->name().toStdString() << endl;
            QString fileName = QString("streamDataBUffer-c%1.dat").arg(_counter);
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            QTextStream out(&file);

            unsigned nPackets = 512;
            size_t headerSize = 16;
            size_t packetSize = 31 * 16 * 2 * sizeof(i16c) + 16;
            unsigned iStart = 1 * 16 * 2 + 0;
            unsigned iEnd = iStart + 16 * 2;
            for (unsigned p = 0; p < nPackets; ++p)
            {
                unsigned offset = (p * packetSize) + headerSize;
                char* packetData = (char*)s->data() + offset;
                //std::cout << "p = " << p << "offset = " << offset << endl;
                i16c* d = reinterpret_cast<i16c*>(packetData);
                for (unsigned jj = iStart; jj < iEnd; jj+=2)
                {
                    out << QString::number(jj) << " ";
                    out << QString::number(d[jj].real()) << " ";
                    out << QString::number(d[jj].imag()) << endl;
                }
            }
        }

        _serveQueue.enqueue(data);
        _counter++;
    } else {
        QMutexLocker writeLocker(&_writeMutex);
        _emptyQueue.push_back(data);
    }
}

int StreamDataBuffer::numberOfActiveChunks() const
{
    return _serveQueue.size();
}
} // namespace pelican

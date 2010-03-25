#ifndef STREAMDATABUFFER_H
#define STREAMDATABUFFER_H

#include "DataBuffer.h"
#include <QQueue>
#include <QObject>

/**
 * @file StreamDataBuffer.h
 */

namespace pelican {

class LockableStreamData;
class Data;
class DataManager;
class LockedData;
class WritableData;

/**
 * @class StreamDataBuffer
 *  
 * @brief
 * Maintain StreamData and memory access.
 *
 * @details
 * Encapsulates memory allocation for streams, with locking and data
 * consistency checking.
 */
class StreamDataBuffer : public DataBuffer
{
    Q_OBJECT

    public:
        /// StreamDataBuffer constructor.
        StreamDataBuffer(const QString& type, DataManager* manager = 0,
                QObject* parent = 0);

        /// StreamDataBuffer destructor.
        ~StreamDataBuffer();

        /// Get the next data object that is ready to be served.
        void getNext(LockedData&);

        /// Get a data object that is ready to be written to.
        WritableData getWritable(size_t size);

        /// Set the data manager to use.
        void setDataManager(DataManager* manager) {_manager = manager;}

    protected slots:
        /// Places the data chunk that emitted the signal on the serve queue.
        void activateData();

        /// Places the data chunk that emitted the signal on the empty queue.
        void deactivateData();

    protected:
        /// Places the given data chunk on the serve queue.
        void activateData(LockableStreamData*);

        /// Places the given data chunk on the empty queue.
        void deactivateData(LockableStreamData*);
        LockableStreamData* _getWritable(size_t size);

    private:
        StreamDataBuffer(const StreamDataBuffer&); // Disallow copying.

    private:
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        QList<LockableStreamData*> _data;
        QQueue<LockableStreamData*> _serveQueue;
        QQueue<LockableStreamData*> _emptyQueue;
        QList<int> _testList;
        DataManager* _manager;
};

} // namespace pelican

#endif // STREAMDATABUFFER_H 

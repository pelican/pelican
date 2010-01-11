#ifndef STREAMDATABUFFER_H
#define STREAMDATABUFFER_H

#include "DataBuffer.h"
#include <QQueue>
#include <QObject>

/**
 * @file StreamDataBuffer.h
 */

namespace pelican {

class StreamData;
class Data;
class DataManager;
class LockedData;
class WritableData;

/**
 * @class StreamDataBuffer
 *  
 * @brief
 *    Maintain StreamData and memory access
 * @details
 *    Encapsulates memory allocation for streams, with locking and data 
 *    consistency checking
 */

class StreamDataBuffer : public DataBuffer
{
    Q_OBJECT

    public:
        StreamDataBuffer(DataManager* manager=0, QObject* parent=0);
        ~StreamDataBuffer();

        /// get the next data object that is ready to be served
        LockedData getNext();

        /// get a data object that is ready to be written to
        WritableData getWritable(size_t size);

        /// set the data manager to use
        void setDataManager(DataManager* manager);

    protected slots:
        void activateData();

    protected:
        void activateData(StreamData*);
        StreamData* _getWritable(size_t size);

    private:
        StreamDataBuffer(const StreamDataBuffer&); // disallow copying

    private:
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        QList<StreamData*> _data;
        QQueue<StreamData*> _serveQueue;
        QQueue<StreamData*> _emptyQueue;
        DataManager* _manager;
};

} // namespace pelican
#endif // STREAMDATABUFFER_H 

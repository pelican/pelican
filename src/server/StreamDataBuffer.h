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
        StreamDataBuffer(QObject* parent=0);
        ~StreamDataBuffer();
        LockedData getNext();
        WritableData getWritable(size_t size);
        void _newData();

    protected slots:
        void activateData(Data*);

    protected:
        void activateData(StreamData*);

    private:
        StreamDataBuffer(const StreamDataBuffer&); // disallow copying

    private:
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        QList<StreamData*> _data;
        QQueue<StreamData*> _serveQueue;
        QQueue<StreamData*> _emptyQueue;
};

} // namespace pelican
#endif // STREAMDATABUFFER_H 

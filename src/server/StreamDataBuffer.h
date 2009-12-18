#ifndef STREAMDATABUFFER_H
#define STREAMDATABUFFER_H

#include "DataBuffer.h"
#include <QMutex>
#include <QQueue>

/**
 * @file StreamDataBuffer.h
 */

namespace pelican {

class StreamData;
class LockedData;

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
    public:
        StreamDataBuffer();
        ~StreamDataBuffer();
        LockedData getNext();

    protected:
//        QMutex _mutex;

    private:
        QQueue<StreamData*> _serveQueue;
};

} // namespace pelican
#endif // STREAMDATABUFFER_H 

#ifndef DATABUFFER_H
#define DATABUFFER_H
#include <QObject>
#include <QMutex>


/**
 * @file DataBuffer.h
 */

namespace pelican {

class WritableData;

/**
 * @class DataBuffer
 *  
 * @brief
 * 
 * @details
 * 
 */

class DataBuffer : public QObject
{
    Q_OBJECT

    public:
        DataBuffer(QObject* parent=0);
        ~DataBuffer();
        WritableData getWritable(size_t size);

    protected:
        QMutex _mutex;

    private:
};

} // namespace pelican
#endif // DATABUFFER_H 

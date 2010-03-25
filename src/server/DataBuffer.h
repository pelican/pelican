#ifndef DATABUFFER_H
#define DATABUFFER_H
#include <QObject>
#include <QMutex>
#include <QString>

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
        /// Constructs a new DataBuffer object.
        DataBuffer(const QString& type, QObject* parent = 0)
        : QObject(parent), _type(type) {}

        /// Destroys the DataBuffer object.
        virtual ~DataBuffer() {}

        virtual WritableData getWritable(size_t size) = 0;

    protected:
        QMutex _mutex;
        QString _type;
};

} // namespace pelican

#endif // DATABUFFER_H 

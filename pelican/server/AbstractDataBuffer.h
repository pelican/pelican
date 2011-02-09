#ifndef ABSTRACTDATABUFFER_H
#define ABSTRACTDATABUFFER_H

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QString>

/**
 * @file AbstractDataBuffer.h
 */

namespace pelican {

class WritableData;

/**
 * @ingroup c_server
 *
 * @class AbstractDataBuffer
 *
 * @brief
 * Interface class for generic data buffers.
 *
 * @details
 * This class provides an abstract interface for accessing chunks of data
 * held in data buffers, and defines the getWritable() method for this purpose.
 *
 * The class is inherited by StreamDataBuffer and ServiceDataBuffer.
 */
class AbstractDataBuffer : public QObject
{
    Q_OBJECT

    public:
        /// Constructs a new abstract data buffer object.
        AbstractDataBuffer(const QString& type, QObject* parent = 0) :
            QObject(parent), _type(type) {}

        /// Destroys the data buffer object.
        virtual ~AbstractDataBuffer() {}

        /// Returns a WritableData object of the specified size from the buffer.
        virtual WritableData getWritable(size_t size) = 0;

    protected:
        QMutex _mutex;
        QMutex _writeMutex;
        QString _type;
};

} // namespace pelican

#endif // ABSTRACTDATABUFFER_H

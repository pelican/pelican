#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H
#include <QUdpSocket>
#include <QString>
#include "DataManager.h"
#include "WritableData.h"

/**
 * @file AbstractChunker.h
 */

namespace pelican {

/**
 * @class AbstractChunker
 *  
 * @brief
 * Base class for all chunker plug-ins.
 *
 * @details
 * Methods on this class are called by the DataListener class
 * which sets up the necessary connections etc.
 */
class AbstractChunker
{
    private:
        DataManager* _dm;
        QString _type;

    public:
        /// Constructs a new AbstractChunker.
        AbstractChunker(const QString& type, DataManager* dm)
            : _dm(dm), _type(type) {}

        /// Destroys the AbstractChunker.
        virtual ~AbstractChunker() {}

        /// Return the type name to be associated with this data.
        const QString& type() const {return _type;}

        /// Create a new socket appropriate to the type expected on the data stream.
        /// By default, the new socket will be a UDP socket.
        virtual QAbstractSocket* newSocket() const {return new QUdpSocket;}

        /// Called whenever there is data ready to be processed.
        /// Inheriting classes should process a complete data chunk
        /// inside this method.
        virtual void next(QAbstractSocket*) = 0;

    protected:
        /// Access to memory to store data is through this interface.
        /// The WritableData object should always be checked with its
        /// isValid() method before use. When the WritableData object goes
        /// out of scope the data will become available to be served
        /// automatically if it is valid.
        WritableData getDataStorage( size_t size ) const {
            return _dm->getWritableData(_type, size);
        }
};

} // namespace pelican

#endif // ABSTRACTCHUNKER_H 

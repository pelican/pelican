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
 * Methods on this class are called by the DataReceiver class
 * which sets up the necessary connections etc.
 */
class AbstractChunker
{
    private:
        DataManager* _dataManager;
        QString _type;
        QString _host;
        quint16 _port;

    protected:
        QIODevice* _device;

    public:
        /// Constructs a new AbstractChunker.
        AbstractChunker(const QString& type, DataManager* dataManager, QString host = "", quint16 port = 0)
            : _dataManager(dataManager), _type(type), _host(host), _port(port), _device(NULL) {}

        /// Destroys the AbstractChunker.
        virtual ~AbstractChunker();

        /// Return the type name to be associated with this data.
        const QString& type() const {return _type;}

        /// Create a new device appropriate to the type expected on the data stream.
        virtual QIODevice* newDevice() = 0;

        /// Called whenever there is data ready to be processed.
        /// Inheriting classes should process a complete data chunk
        /// inside this method.
        virtual void next(QIODevice*) = 0;

        /// Returns the host.
        const QString& host() {return _host;}

        /// Returns the port.
        quint16 port() {return _port;}

    protected:
        /// Access to memory to store data is through this interface.
        /// The WritableData object should always be checked with its
        /// isValid() method before use. When the WritableData object goes
        /// out of scope the data will become available to be served
        /// automatically if it is valid.
        WritableData getDataStorage(size_t size) const {
            return _dataManager->getWritableData(_type, size);
        }
};

} // namespace pelican

#endif // ABSTRACTCHUNKER_H 

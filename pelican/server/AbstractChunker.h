#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H

#include "pelican/server/DataManager.h"
#include "pelican/server/WritableData.h"
#include "pelican/utility/FactoryRegistrar.h"

#include <QUdpSocket>
#include <QString>


/**
 * @file AbstractChunker.h
 */

namespace pelican {

/**
 * This macro is used to register the named chunker type with the factory.
 * It should be used within the global scope of the chunker's source file.
 */
#define PELICAN_DECLARE_CHUNKER(type) PELICAN_DECLARE(AbstractChunker, type)


class ConfigNode;

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
        QIODevice* _device;

    public:
        /// Constructs a new AbstractChunker.
        AbstractChunker(const QString& type, QString host = "", quint16 port = 0)
            : _dataManager(0), _type(type), _host(host), _port(port), _device(0) {}

        /// Constructs a new AbstractChunker.
        PELICAN_CONSTRUCT_TYPES(ConfigNode)
        AbstractChunker(const ConfigNode& config);

        AbstractChunker() :
            _dataManager(0), _type(""), _host(""), _port(0), _device(0) {}

        /// Destroys the AbstractChunker.
        virtual ~AbstractChunker();

        /// Returns the host.
        const QString& host() {return _host;}

        /// Create a new device appropriate to the type expected on the data stream.
        virtual QIODevice* newDevice() = 0;

        /// Called whenever there is data ready to be processed.
        /// Derived classes must process a complete data chunk in this method.
        virtual void next(QIODevice*) = 0;

        /// Returns the port.
        quint16 port() {return _port;}

        /// Sets the data manager.
        void setDataManager(DataManager* dataManager) {
            _dataManager = dataManager;
        }

        /// Sets the device pointer.
        void setDevice(QIODevice* device) {_device = device;}

        /// Set the IP address to listen on for data.
        void setHost(const QString& ipaddress) {_host = ipaddress;}

        /// Set the port to listen on for data.
        void setPort(quint16 port) {_port = port;}

        /// Sets the type name to be associated with this data.
        void setType(const QString& type) {_type = type;}

        /// Return the type name to be associated with this data.
        const QString& type() const {return _type;}

    protected:
        /// Access to memory to store data is through this interface.
        /// The WritableData object should always be checked with its
        /// isValid() method before use. When the WritableData object goes
        /// out of scope the data will become available to be served
        /// automatically if it is valid.
        WritableData getDataStorage(size_t size) const;
};

} // namespace pelican

#endif // ABSTRACTCHUNKER_H

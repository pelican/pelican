#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H

#include "pelican/server/DataManager.h"
#include "pelican/server/WritableData.h"
#include "pelican/utility/FactoryRegistrar.h"

#include <QtNetwork/QUdpSocket>
#include <QtCore/QString>
#include <QtCore/QStringList>


/**
 * @file AbstractChunker.h
 */

namespace pelican {

// Macro is used to register the named chunker type.
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
        QString _host;      ///< Host address for incoming connections.
        quint16 _port;      ///< Port for incoming connections.
        QIODevice* _device; ///< QIODevice for receiving the incoming data.

        DataManager* _dataManager; ///< Data manager used by the chunker to access writable data.
        QStringList _chunkTypes;   ///< Data chunk data types written to by the chunker.

        bool _active;

    public:
        /// Constructs a new AbstractChunker.
        PELICAN_CONSTRUCT_TYPES(ConfigNode)
        AbstractChunker(const ConfigNode& config);

        /// Constructs a new AbstractChunker (used in testing).
        AbstractChunker() : _host(""), _port(0), _device(0), _dataManager(0),
        _chunkTypes(QStringList::QStringList()), _active(true)
        {}

        /// Destroys the AbstractChunker.
        virtual ~AbstractChunker();

    public:
        /// Create a new device appropriate to the type expected on the data stream.
        virtual QIODevice* newDevice() = 0;

        /// Called whenever there is data ready to be processed.
        /// Derived classes must process a complete data chunk in this method.
        virtual void next(QIODevice*) = 0;

        /// Sets the data manager.
        void setDataManager(DataManager* dataManager)
        { _dataManager = dataManager; }

        /// Sets the device pointer.
        void setDevice(QIODevice* device) { _device = device; }

        /// Returns the QIODevice pointer previously set with setDevice().
        QIODevice* getDevice() { return _device; };

        /// Set the IP address to listen on for data.
        void setHost(const QString& ipaddress) { _host = ipaddress; }

        /// Returns the host.
        const QString& host() { return _host; }

        /// Set the port to listen on for data.
        void setPort(quint16 port) { _port = port; }

        /// Returns the port.
        quint16 port() { return _port; }

        /// Sets the type name to be associated with this data.
        void setChunkTypes(const QStringList& types) { _chunkTypes = types; }

        /// Adds a chunk type to the types written to by the chucker.
        void addChunkType(const QString& type) { _chunkTypes.append(type); }

        /// Return the type name to be associated with this data.
        const QStringList& chunkTypes() const { return _chunkTypes; }

        /// Stops the chunker.
        void stop() { _active = false; }

    protected:
        /// Access to memory to store data is through this interface.
        /// The WritableData object should always be checked with its
        /// isValid() method before use. When the WritableData object goes
        /// out of scope the data will become available to be served
        /// automatically if it is valid.
        WritableData getDataStorage(size_t size) const;

        /// Overloaded method specifying the chunk type of the buffer.
        WritableData getDataStorage(size_t size, const QString& chunkType) const;

        /// Returns the state of the chunker (running or not).
        bool isActive() { return _active; }
};

} // namespace pelican
#endif // ABSTRACTCHUNKER_H

#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H

/**
 * @file AbstractChunker.h
 */

#include "pelican/server/DataManager.h"
#include "pelican/server/WritableData.h"
#include "pelican/utility/FactoryRegistrar.h"

#include <QtNetwork/QUdpSocket>
#include <QtCore/QList>
#include <QtCore/QString>

namespace pelican {

// Macro is used to register the named chunker type.
#define PELICAN_DECLARE_CHUNKER(type) PELICAN_DECLARE(AbstractChunker, type)

class ConfigNode;

/**
 * @ingroup c_server
 *
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

        DataManager* _dataManager;  ///< Data manager used to access writable data objects.
        QList<QString> _chunkTypes; ///< List of the chunk data types written.

        bool _active;

    public:
        /// Constructs a new AbstractChunker.
        PELICAN_CONSTRUCT_TYPES(ConfigNode)
        AbstractChunker(const ConfigNode& config);

        /// Constructs a new AbstractChunker (used in testing).
        AbstractChunker() : _host(""), _port(0), _dataManager(0),
        _active(false)
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

        /// Set the IP address to listen on for data.
        void setHost(const QString& ipaddress) { _host = ipaddress; }

        /// Returns the host.
        const QString& host() { return _host; }

        /// Set the port to listen on for data.
        void setPort(quint16 port) { _port = port; }

        /// Returns the port.
        quint16 port() { return _port; }

        /// Sets the type name to be associated with this data.
        void setChunkTypes(const QList<QString> & types) { _chunkTypes = types; }

        /// Adds a chunk type to the types written to by the chucker.
        void addChunkType(const QString& type) { _chunkTypes.append(type); }

        /// Return the type name to be associated with this data.
        const QList<QString> & chunkTypes() const { return _chunkTypes; }

        /// Returns the type name associated with the data.
        /// NOTE: This will be depricated. DO NOT USE!.
#pragma deprecated(type)
        const QString& type() const { return _chunkTypes[0]; }

        /// Stops the chunker.
        void stop() { _active = false; }
        void activate() { _active = true; }

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
        bool isActive() const { return _active; }

};

} // namespace pelican
#endif // ABSTRACTCHUNKER_H

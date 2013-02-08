/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ABSTRACTCHUNKER_H
#define ABSTRACTCHUNKER_H

/**
 * @file AbstractChunker.h
 */

#include "server/DataManager.h"
#include "server/WritableData.h"
#include "utility/FactoryRegistrar.h"

#include <QtNetwork/QUdpSocket>
#include <QtCore/QList>
#include <QtCore/QString>

// Macro is used to register the named chunker type.
#define PELICAN_DECLARE_CHUNKER(type) PELICAN_DECLARE(pelican::AbstractChunker, type)

namespace pelican {

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

        QHash< QString, QString > _adapterTypes;

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
        void setDataManager(DataManager* dataManager);

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

        /// associates a default adapter to a named
        /// stream. Note that the stream (or chunkType)
        /// must already be registered
        void setDefaultAdapter( const QString& adapter );
        void setDefaultAdapter( const QString& adapter,
                                const QString& stream );

        /// Returns the type name associated with the data.
        /// NOTE: This will be depricated. DO NOT USE!.
//#pragma deprecated(type)
        const QString& type() const __attribute__ ((deprecated))
        { return _chunkTypes[0]; }

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


        const QHash<QString, QString>& defaultAdapters() const {
             return _adapterTypes;
        };

};

} // namespace pelican
#endif // ABSTRACTCHUNKER_H

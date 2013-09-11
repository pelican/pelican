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

#ifndef CHUNKERMANAGER_H
#define CHUNKERMANAGER_H

/**
 * @file ChunkerManager.h
 */

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtCore/QVector>
#include <boost/shared_ptr.hpp>

#include "utility/FactoryConfig.h"
#include "server/DataReceiver.h"
#include "utility/Config.h"


namespace pelican {

class AbstractChunker;
class DataManager;

/**
 * @ingroup c_server
 *
 * @class ChunkerManager
 *
 * @brief
 * Manage and operate chunkers.
 *
 * @details
 * Anything coming through a stream data chunker will be associated with the
 * current version of service data service data has no such association.
 */

class ChunkerManager
{
    public:
        /// Creates a chunker manager.
        /// DEPRECATED
        ChunkerManager(const Config* config, const QString& section = "server");

        /// Creates a chunker manager.
        ChunkerManager(const Config* config,  const Config::TreeAddress& base);

        /// Destroys the chunker manager.
        ~ChunkerManager();

    public:
        /// Initialise the passed data manager to support the contained chunkers.
        bool init(DataManager& dm);

        /// Adds a stream chunker by class name (\p type).
        AbstractChunker* addStreamChunker(QString type, QString name = QString());

        /// Adds a service chunker by class name (\p type).
        AbstractChunker* addServiceChunker(QString type, QString name = QString());

        /// Returns the list of chunkers.
        const QSet<AbstractChunker* >& chunkers() const { return _chunkers; };

        /// return the current device associated with the specified
        //  chunker
        QIODevice* getCurrentDevice( AbstractChunker* chunker ) const;

        /// Returns true if all the data chunkers are operating, otherwise false.
        bool isRunning() const;

    private:
        // Adds an allocated chunker to the manager.
        void _addChunker( AbstractChunker* chunker);

    private:
        FactoryConfig<AbstractChunker> *_factory;
        QMap<QPair<QString,quint16>,AbstractChunker* > _chunkerPortMap;
        QSet<AbstractChunker* > _chunkers;
        QSet<QString> _streamDataTypes;
        QSet<QString> _serviceDataTypes;
        QHash<AbstractChunker*, DataReceiver*> _dataReceivers;
};

} // namespace pelican
#endif // CHUNKERMANAGER_H

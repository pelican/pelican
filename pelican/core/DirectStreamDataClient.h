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

#ifndef DIRECTSTREAMDATACLIENT_H
#define DIRECTSTREAMDATACLIENT_H

/**
 * @file DirectStreamDataClient.h
 */

#include "core/AbstractAdaptingDataClient.h"

namespace pelican {

class ChunkerManager;
class DataManager;
class Config;

/**
 * @ingroup c_core
 *
 * @class DirectStreamDataClient
 *
 * @brief
 * A data client that connects directly to one or more data streams.
 *
 * @details
 * This data client connects directly to one or more data streams using
 * specialist chunkers, rather than to the PelicanServer.
 * This is useful when the data rates on the stream can be handled in a single
 * pipeline.
 *
 * Add the chunkers using the addChunker() method in a derived class
 * constructor.
 */
class DirectStreamDataClient : public AbstractAdaptingDataClient
{
    public:
        /// Constructs the direct stream data client.
        DirectStreamDataClient(const ConfigNode& configNode,
                const DataTypes& types, const Config* config
                );

        /// Destroys the direct stream data client.
        virtual ~DirectStreamDataClient();

        /// Obtains a hash of data blobs required by the pipeline.
        virtual DataBlobHash getData(DataBlobHash& dataHash);
        virtual const DataSpec& dataSpec() const;

        /// Sets the chunker for the given data type.
        void addChunker(const QString& dataType, const QString& chunkerType,
                const QString& chunkerName = QString());

        /// Sets the service chunker for the given data type.
        void addServiceChunker(const QString& chunkerType,
                const QString& chunkerName = QString());

        /// Sets the stream chunker for the given data type.
        void addStreamChunker(const QString& chunkerType,
                const QString& chunkerName = QString());

    private:
        /// Initialises the direct stream data client.
        void _init(QHash<QString, DataBlob*>& dataHash);

    private:
        bool _started;
        int _nPipelines;
        ChunkerManager* _chunkerManager;
        DataManager* _dataManager;
};

PELICAN_DECLARE_CLIENT(DirectStreamDataClient)

} // namespace pelican

#endif // DIRECTSTREAMDATACLIENT_H

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

#ifndef ABSTRACTADAPTINGDATACLIENT_H
#define ABSTRACTADAPTINGDATACLIENT_H


#include "AbstractDataClient.h"

/**
 * @file AbstractAdaptingDataClient.h
 */

namespace pelican {

/**
 * @class AbstractAdaptingDataClient
 *
 * @brief
 *     Base class for DataClient's that use adapters
 *     This is probably the base class you should be using for
 *     a DataClient
 * @details
 *
 */

class AbstractAdaptingDataClient : public AbstractDataClient
{
    public:
        typedef AbstractDataClient::DataBlobHash DataBlobHash;

    public:
        //PELICAN_CONSTRUCT_TYPES(const ConfigNode&, const DataTypes&, const Config*)
        AbstractAdaptingDataClient( const ConfigNode& configNode,
                                    const DataTypes& types,
                                    const Config* config
                                    );
        ~AbstractAdaptingDataClient();

        /// Returns the type of data associated with the data name
        /// (stream/service/unknown)
        AbstractAdapter::AdapterType_t type(const QString& dataName) const
        { return _dataReqs.type(dataName); }

        /// reconfigure the client to deliver the specified
        /// data requirements
        virtual void reset( const QList<DataSpec>& specification );

    protected:
        /// Adapts (de-serialises) stream data.
        DataBlobHash adaptStream(QIODevice& device, const StreamData* d,
                DataBlobHash& dataHash);

        /// Adapts (de-serialises) service data.
        DataBlobHash adaptService(QIODevice& device, const DataChunk* sd,
                DataBlobHash& dataHash);

        /// Returns a pointer to the configuration node.
        const ConfigNode& configNode() const {return _configNode;}

        /// Returns the adapter for service data of the required type.
        AbstractServiceAdapter* serviceAdapter(const QString& type) const
        { return _dataReqs.serviceAdapter(type); }

        /// Returns the adapter for stream data of the required type.
        AbstractStreamAdapter* streamAdapter(const QString& type) const
        { return _dataReqs.streamAdapter(type); }


    private:
        DataTypes _dataReqs;    ///< The DataTypes and requirements.
};

} // namespace pelican
#endif // ABSTRACTADAPTINGDATACLIENT_H

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

#include "AbstractAdaptingDataClient.h"
#include "core/AbstractStreamAdapter.h"
#include "core/AbstractServiceAdapter.h"
#include "comms/StreamData.h"
#include "data/DataBlob.h"


namespace pelican {


/**
 *@details AbstractAdaptingDataClient
 */
AbstractAdaptingDataClient::AbstractAdaptingDataClient( const ConfigNode& configNode,
                const DataTypes& requirements,
                const Config* config )
    : AbstractDataClient( configNode, requirements, config )
{
    _dataReqs = requirements;
}

/**
 *@details
 */
AbstractAdaptingDataClient::~AbstractAdaptingDataClient()
{
}

void AbstractAdaptingDataClient::reset( const QList<DataSpec>& requirements  ) {

    AbstractDataClient::reset(requirements);

    // remove any previous settings
    //_dataReqs.clear();
    _dataReqs.addData( requirements );

    // check we have adapters for each type
    foreach (const QString& dataType, _requireSet) {
        if (!_dataReqs.adapterAvailable(dataType))
            throw QString("AbstractAdaptingDataClient: Unable to find adapter for "
                    "data type '%1'.").arg(dataType);
    }
}

/**
 * @details
 * Adapts (de-serialises) stream data into data blobs.
 *
 * @param device
 * @param sd
 * @param dataHash
 *
 * @return
 */
AbstractDataClient::DataBlobHash AbstractAdaptingDataClient::adaptStream(
        QIODevice& device, const StreamData* sd, DataBlobHash& dataHash)
{
    QHash<QString, DataBlob*> validData;

    const QString& type = sd->name();
    dataHash[type]->setVersion(sd->id());
    AbstractStreamAdapter* adapter = streamAdapter(type);
    Q_ASSERT( adapter != 0 );
    adapter->config( dataHash[type], sd->size(), dataHash );
    adapter->deserialise(&device);
    validData.insert(type, dataHash.value(type));

    return validData;
}

/**
 * @details
 * Adapts (deserialises) service data into data blobs.
 *
 * @param device
 * @param d
 * @param dataHash
 *
 * @return
 */
AbstractDataClient::DataBlobHash AbstractAdaptingDataClient::adaptService(
        QIODevice& device, const DataChunk* d, DataBlobHash& dataHash)
{
    QHash<QString, DataBlob*> validData;
    QString type = d->name();
    AbstractServiceAdapter* adapter = serviceAdapter(type);
    Q_ASSERT(adapter != 0 );
    dataHash[type]->setVersion(d->id());
    adapter->config(dataHash[type], d->size() );
    adapter->deserialise(&device);
    validData.insert(type, dataHash.value(type));
    return validData;
}

} // namespace pelican

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

#ifndef ABSTRACT_DATA_CLIENT_H
#define ABSTRACT_DATA_CLIENT_H

/**
 * @file AbstractDataClient.h
 */

#include "core/DataTypes.h"
#include "utility/FactoryRegistrar.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"
#include "utility/FactoryConfig.h"

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QString>

class QIODevice;

// This macro is used to register the named client type.
#define PELICAN_DECLARE_CLIENT(type) PELICAN_DECLARE(pelican::AbstractDataClient, type)

namespace pelican {

class AbstractAdapter;
class DataBlob;
class StreamData;
class DataChunk;

/**
 * @ingroup c_core
 *
 * @class AbstractDataClient
 *
 * @brief
 * Abstract base class for all data clients.
 *
 * @details
 * Data clients fetch data in order to make it available to the pipelines via
 * the PipelineDriver. The data client is created by the pipeline driver
 * after determining the data requirements of all of the pipelines.
 *
 * In order to create a data client, inherit this class and implement the
 * getData() method.
 *
 * \par Common configuration:
 *
 * Under its root XML configuration node, each data client must have a list of
 * data types and their associated adapters. This information is then used
 * by the data client factory for automatically creating the adapters attached
 * to each data type.
 *
 * For example:
 *
 * \code
 *  <data type="VisibilityData" adapter="AdapterStationVisibilities"/>
 *  <data type="AntennaPositions" adapter="AdapterStationAntennas"/>
 * \endcode
 */

class AbstractDataClient
{
    public:
        typedef QHash<QString, DataBlob*> DataBlobHash;

    public:

        /// Data client constructor.
        AbstractDataClient(const ConfigNode& configNode, const DataTypes& types,
                           const Config* config
                );

        /// Data client destructor.
        virtual ~AbstractDataClient();
        virtual void reset( const QList<DataSpec>& specification );

        /// get the Data specifications that the client provides
        virtual const DataSpec& dataSpec() const = 0;

        PELICAN_CONSTRUCT_TYPES(const ConfigNode&, const DataTypes&, const Config*)

    public:
        /// Retrieves data requested by pipelines and fills the given data hash,
        /// and returns another hash of valid data.
        /// (\todo what is valid data in this context?)
        virtual DataBlobHash getData(DataBlobHash&) = 0;

        /// Returns the list of data requirements for each pipeline.
        const QList<DataSpec>& dataRequirements() { return _dataRequirements; }


    protected:
        /// Writes a message to the log.
        void log(const QString& msg);

        /// Returns a pointer to the configuration node.
        const ConfigNode& configNode() const {return _configNode;}

/*
        /// Adapts (de-serialises) stream data.
        DataBlobHash adaptStream(QIODevice& device, const StreamData* d,
                DataBlobHash& dataHash);

        /// Adapts (de-serialises) service data.
        DataBlobHash adaptService(QIODevice& device, const DataChunk* sd,
                DataBlobHash& dataHash);


        /// Returns the adapter for service data of the required type.
        AbstractServiceAdapter* serviceAdapter(const QString& type) const
        { return _dataReqs.serviceAdapter(type); }

        /// Returns the adapter for stream data of the required type.
        AbstractStreamAdapter* streamAdapter(const QString& type) const
        { return _dataReqs.streamAdapter(type); }
*/

    protected:
        ConfigNode _configNode; ///< The configuration node for the data client.
        const Config* _config;
        QSet<QString> _requireSet;

    private:
        QList<DataSpec> _dataRequirements;
};

} // namespace pelican
#endif // ABSTRACT_DATA_CLIENT_H

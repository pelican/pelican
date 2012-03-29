#ifndef DIRECTSTREAMDATACLIENT_H
#define DIRECTSTREAMDATACLIENT_H

/**
 * @file DirectStreamDataClient.h
 */

#include "pelican/core/AbstractAdaptingDataClient.h"

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
        DataBlobHash getData(DataBlobHash& dataHash);

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

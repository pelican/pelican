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

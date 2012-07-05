#include "AbstractAdaptingDataClient.h"
#include "pelican/core/AbstractStreamAdapter.h"
#include "pelican/core/AbstractServiceAdapter.h"
#include "pelican/comms/StreamData.h"
#include "pelican/data/DataBlob.h"


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
    // check we have adapters for each type
    foreach (const QString& dataType, _requireSet) {
        if (!_dataReqs.adapterAvailable(dataType))
            throw QString("AbstractAdaptingDataClient: Unable to find adapter for "
                    "data type '%1'.").arg(dataType);
    }
}

/**
 *@details
 */
AbstractAdaptingDataClient::~AbstractAdaptingDataClient()
{
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

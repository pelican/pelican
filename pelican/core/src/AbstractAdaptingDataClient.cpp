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
                const QList<DataRequirements>& requirements,
                const Config* config, FactoryConfig<AbstractAdapter>* adapterFactory )
    : AbstractDataClient( configNode, requirements, config, adapterFactory )
{
    _dataReqs.addData(requirements);

    // Find the configuration information for adapters.
    QHash<QString, QString> adapterNames = configNode.getOptionHash("data", "type", "adapter");

    // Construct the adapters and add them to the DataTypes structure.
    foreach (const DataRequirements& req, requirements)
    {
        QSet<QString> all = req.allData();
        foreach (const QString& dataType, all)
        {
            if (!adapterNames.contains(dataType))
                throw QString("DataClientFactory: Unable to find adapter for "
                        "data type '%1'.").arg(dataType);
            AbstractAdapter* adapter =
                    _adapterFactory->create(adapterNames.value(dataType), 
                                configNode.getNamedOption("data","name","") );
            _dataReqs.setAdapter(dataType, adapter);
        }
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

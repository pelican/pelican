#include <iostream>
#include "core/AbstractDataClient.h"
#include "adapters/AdapterFactory.h"
#include "adapters/AbstractStreamAdapter.h"
#include "adapters/AbstractServiceAdapter.h"
#include "data/DataRequirements.h"
#include "comms/Data.h"
#include "comms/StreamData.h"
#include "utility/ConfigNode.h"
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 * The abstract data client constructor takes a reference to the data
 * client configuration node (\p config).
 *
 * @param[in] config Reference to the data client configuration node.
 * @param[in] types  Object containing the requirements and adapters for each pipeline.
 */
AbstractDataClient::AbstractDataClient(const ConfigNode& config, const DataTypes& types)
    : _dataReqs( types ), _configNode(config)
{
    // Quick sanity check.
    if ( _dataReqs.dataRequirements().size()== 0 )
        throw( QString("No data requirements specified") );

    // Construct the total set of requirements.
    foreach( const DataRequirements dr, dataRequirements() ) {
        _requireSet += dr.allData();
    }
}

/**
 * @details
 * Destroys the data client.
 */
AbstractDataClient::~AbstractDataClient()
{
}

/**
 * @details
 * Writes a message to the log (currently stderr).
 */
void AbstractDataClient::log(const QString& msg)
{
    std::cerr << msg.toStdString() << std::endl;
}


QHash<QString, DataBlob*> AbstractDataClient::adaptStream(QIODevice& device, const StreamData* sd, QHash<QString, DataBlob*>& dataHash )
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

QHash<QString, DataBlob*> AbstractDataClient::adaptService(QIODevice& device, const Data* d, QHash<QString, DataBlob*>& dataHash )
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

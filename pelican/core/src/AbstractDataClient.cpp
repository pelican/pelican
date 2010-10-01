#include "pelican/core/AbstractDataClient.h"
#include "pelican/core/AbstractStreamAdapter.h"
#include "pelican/core/AbstractServiceAdapter.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"
#include "pelican/comms/Data.h"
#include "pelican/comms/StreamData.h"
#include "pelican/utility/ConfigNode.h"

#include <QtCore/QtGlobal>

#include <iostream>
using std::cout;
using std::endl;

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 * The abstract data client constructor takes a reference to the data
 * client configuration node (\p config).
 *
 * @param[in] config Reference to the data client configuration node.
 */
AbstractDataClient::AbstractDataClient(const ConfigNode& configNode,
        const DataTypes& types, const Config* config) :
        _configNode(configNode), _dataReqs(types), _config(config)
{
    // Quick sanity check.
    if (_dataReqs.dataRequirements().size() == 0)
        throw QString("AbstractDataClient: "
                "No data requirements specified");

    // Construct the total set of requirements.
    foreach (const DataRequirements dr, dataRequirements()) {
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

/**
 * @details
 * Adapts (deserialises) stream data into data blobs.
 *
 * @param device
 * @param sd
 * @param dataHash
 * @return
 */
QHash<QString, DataBlob*> AbstractDataClient::adaptStream(QIODevice& device,
        const StreamData* sd, QHash<QString, DataBlob*>& dataHash )
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
 * @return
 */
QHash<QString, DataBlob*> AbstractDataClient::adaptService(QIODevice& device,
        const Data* d, QHash<QString, DataBlob*>& dataHash )
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

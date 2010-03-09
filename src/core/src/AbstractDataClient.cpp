#include "core/AbstractDataClient.h"
#include "adapters/AdapterFactory.h"
#include "data/DataRequirements.h"
#include "utility/ConfigNode.h"
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * This creates a new abstract data client.
 * The abstract data client constructor takes a reference to the data
 * client configuration node (\p config), a pointer to the data
 * adapter factory (\p adapterFactory), and a list of data
 * requirements for each pipeline (\p dataRequirements).
 *
 * @param[in] config Reference to the data client configuration node.
 * @param[in] dataType object containing the requirements and adappters for each pipeline.
 */
AbstractDataClient::AbstractDataClient(const ConfigNode& config, const DataTypes& types)
    : _dataReqs( types ), _configNode(&config)
{
    // Quick sanity check
    if ( _dataReqs.dataRequirements().size()== 0 )
        throw( QString("No data requirements specified") );

}

/**
 * @details
 * Destroys the data client.
 */
AbstractDataClient::~AbstractDataClient()
{
}

AbstractServiceAdapter* AbstractDataClient::serviceAdapter(const QString& type) const
{
    return _dataReqs.serviceAdapter(type);
}

AbstractStreamAdapter* AbstractDataClient::streamAdapter(const QString& type) const
{
    return _dataReqs.streamAdapter(type);
}

} // namespace pelican

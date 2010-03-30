#include <iostream>
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

} // namespace pelican

#include "core/AbstractDataClient.h"
#include "data/DataRequirements.h"

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
 * @param[in] adapterFactory Pointer to the adapter factory.
 * @param[in] dataRequirements List of data requirements required per pipeline.
 */
AbstractDataClient::AbstractDataClient(const ConfigNode& config,
        AdapterFactory* adapterFactory,
        QList<DataRequirements>& dataRequirements
){
    _config = &config;
    _adapterFactory = adapterFactory;
    _dataRequirements = dataRequirements;
}

/**
 * @details
 * Destroys the data client.
 */
AbstractDataClient::~AbstractDataClient()
{
}


} // namespace pelican

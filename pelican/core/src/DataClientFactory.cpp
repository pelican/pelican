#include "pelican/core/DataClientFactory.h"
#include "pelican/core/FileDataClient.h"
#include "pelican/core/PelicanServerClient.h"
#include "pelican/core/DataTypes.h"
#include "pelican/data/DataRequirements.h"

#include <iostream>

namespace pelican {

/**
 * @details
 * Method to create a pointer to a data client of the specified \p type.
 *
 * @param[in] type          The type of data client object to create.
 * @param[in] requirements  The data sets to be provided by the client.
 * @param[in] name          An optional specific named configuration to use.
 */
AbstractDataClient* DataClientFactory::create(const QString& type, const QString& name)
{
    // Create a DataTypes object from the DataRequirements.
    DataTypes dataTypes( conf(type,name), _adapterFactory );

    // Call the base class implementation and set the data requirements.
    AbstractDataClient* client = FactoryConfig<AbstractDataClient>::create(
            type, dataTypes, _config , name);

    return client;
}

} // namespace pelican

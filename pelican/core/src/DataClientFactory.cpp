#include "pelican/core/DataClientFactory.h"
#include "pelican/core/FileDataClient.h"
#include "pelican/core/PelicanServerClient.h"
#include "pelican/core/DataTypes.h"
#include "pelican/data/DataRequirements.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Method to create a pointer to a data client of the specified \p type.
 *
 * @param[in] type          The type of data client object to create.
 * @param[in] requirements  The data sets to be provided by the client.
 * @param[in] name          An optional specific named configuration to use.
 */
AbstractDataClient* DataClientFactory::create(const QString& type,
        const QList<DataRequirements>& requirements, const QString& name)
{
    // Create a DataTypes object from the DataRequirements.
    DataTypes dataTypes;
    foreach (const DataRequirements& req, requirements)
        dataTypes.addData(req);

    // Find the configuration information for adapters.
    QHash<QString, QString> adapterNames = configuration(type, name).
            getOptionHash("data", "type", "adapter");

    // Construct the adapters and add them to the DataTypes structure.
    foreach (const DataRequirements& req, requirements)
    {
        QSet<QString> all = req.streamData() + req.serviceData();
        foreach (const QString& dataType, all)
        {
            if (!adapterNames.contains(dataType))
                throw QString("DataClientFactory: Unable to find adapter for "
                        "data type '%1'.").arg(dataType);
            AbstractAdapter* adapter =
                    _adapterFactory->create(adapterNames.value(dataType), "");
            dataTypes.setAdapter(dataType, adapter);
        }
    }

    // Call the base class implementation and set the data requirements.
    AbstractDataClient* client = Factory<AbstractDataClient>::create(type, name);
    client->setManagers(_config);
    client->setDataRequirements(dataTypes);
    return client;
}

} // namespace pelican

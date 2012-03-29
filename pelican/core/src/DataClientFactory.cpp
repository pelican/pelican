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
AbstractDataClient* DataClientFactory::create(const QString& type,
        const QList<DataRequirements>& requirements, const QString& name)
{
    // Create a DataTypes object from the DataRequirements.
    DataTypes dataTypes;
    foreach (const DataRequirements& req, requirements)
        dataTypes.addData(req);

    // Find the configuration information for adapters.
    QHash<QString, QString> adapterNames = conf(type, name).
            getOptionHash("data", "type", "adapter");

    // Construct the adapters and add them to the DataTypes structure.
    foreach (const DataRequirements& req, requirements)
    {
        QSet<QString> all = req.allData();
        foreach (const QString& dataType, all)
        {
            //if (!adapterNames.contains(dataType))
            //    throw QString("DataClientFactory: Unable to find adapter for "
            //            "data type '%1'.").arg(dataType);
            if (adapterNames.contains(dataType)) {
                AbstractAdapter* adapter =
                        _adapterFactory->create(adapterNames.value(dataType), 
                                    conf(type, name).getNamedOption("data","name","") );
                dataTypes.setAdapter(dataType, adapter);
            }
        }
    }

    // Call the base class implementation and set the data requirements.
    AbstractDataClient* client = FactoryConfig<AbstractDataClient>::create(
            type, dataTypes, _config , name);

    return client;
}

} // namespace pelican

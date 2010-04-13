#include "pelican/core/DataClientFactory.h"
#include "pelican/core/FileDataClient.h"
#include "pelican/core/PelicanServerClient.h"
#include "pelican/core/DataTypes.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/adapters/AdapterFactory.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 *@details DataClientFactory
 */
DataClientFactory::DataClientFactory(const Config* config, const Config::TreeAddress& base, AdapterFactory* afactory)
    : AbstractFactory(config, base), _adapterFactory(afactory)
{
}

/**
 *@details
 */
DataClientFactory::~DataClientFactory()
{
    foreach(AbstractDataClient* client, _clients)
        delete client;
}


/**
 * @details
 * Method to create a pointer to a data client of the specified /p type.
 *
 * TODO: ??! DOC GOES HERE ??!!
 *
 * @param[in] type ?? DOC!!
 * @param[in] requirements ?? DOC!!
 * @param[in] name ?? DOC!!
 */
AbstractDataClient* DataClientFactory::create(const QString& type,
        const QList<DataRequirements>& requirements, const QString& name)
{
    // Create a DataTypes Object from the DataRequirements
    DataTypes dataTypes;
    foreach (const DataRequirements& req, requirements)
    {
        dataTypes.addData(req);
    }

    // Find the configuration information for adapters
    Config::TreeAddress address = configRoot();
    address.append(QPair<QString, QString>(type, ""));
    ConfigNode element = _config->get(address);
    QHash<QString, QString> adapterNames = element.getOptionHash("pelican/data",
            "type", "adapter");

    // Construct the adapters and add them to the DataTypes structure
    foreach (const DataRequirements& req, requirements)
    {
        QSet<QString> all = req.streamData();
        all.unite(req.serviceData());
        foreach (const QString& dataType, all)
        {
            if(!adapterNames.contains(dataType))
                throw("pelican/dataClientFactory: Unable to find adapter for data"
                        " type \"" + dataType + "\"");
            AbstractAdapter* adapter =
                    _adapterFactory->create(adapterNames.value(dataType), "");
            dataTypes.setAdapter(dataType, adapter);
        }
    }

    return create(type, dataTypes, name);
}


/**
 * @details
 * Method to create a pointer to a data client of the specified /p type
 *
 *
 * @param[in] type : The type of data client object to create
 * @param[in] requirements : the data sets to be provided by the client
 * @param[in] name : a specific named configuration to use
 */
AbstractDataClient* DataClientFactory::create(const QString& type,
        const DataTypes& requirements, const QString& name)
{
    // Create the required data client
    if (type == "FileDataClient" ) {
        _clients.append(new FileDataClient(configuration( type, name),
                requirements));
    }
    else if (type == "PelicanServerClient" ) {
        _clients.append(new PelicanServerClient(configuration( type, name),
                requirements));
    }
    else {
        throw QString("pelican/dataClientFactory: Unknown data client type: ") + type;
    }
    return _clients.last();
}

} // namespace pelican

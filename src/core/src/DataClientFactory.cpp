#include "DataClientFactory.h"
#include "FileDataClient.h"
#include "PelicanServerClient.h"
#include "DataTypes.h"
#include "data/DataRequirements.h"
#include "adapters/AdapterFactory.h"


#include "utility/memCheck.h"

namespace pelican {

/**
 *@details DataClientFactory 
 */
DataClientFactory::DataClientFactory(const Config* config, const Config::TreeAddress_t& base, AdapterFactory* afactory)
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

AbstractDataClient* DataClientFactory::create( const QString& type, const QList<DataRequirements>& requirements, const QString& name)
{
    // Create a DataTypes Object from the DataRequirements
    DataTypes dt;
    foreach( const DataRequirements& req, requirements )
    {
        dt.addData(req);
    }

    // find the configuration information for adapters
    Config::TreeAddress_t address = configRoot();
    address.append(QPair<QString, QString>(type, ""));
    ConfigNode element = _config->get(address);
    QHash<QString, QString> adapterNames = element.getOptionHash("data", "type", "adapter");

    // construct the adapters and add them to the DataTypes structure
    foreach( const DataRequirements& req, requirements )
    {
        QSet<QString> all = req.streamData();
        all.unite( req.serviceData() );
        foreach( const QString& type, all )
        {
            if( ! adapterNames.contains(type) )
                throw("DataClientFactory: Unable to find adapter for data type \"" + type + "\"");
            AbstractAdapter* adapter = _adapterFactory->create(adapterNames.value(type), "");
            dt.setAdapter( type, adapter );
        }
    }

    return create( type, dt, name);
}

AbstractDataClient* DataClientFactory::create( const QString& type, const DataTypes& requirements, const QString& name)
{
    /* Create the required data client */
    if (type == "FileDataClient" ) {
        _clients.append( new FileDataClient( configuration( type, name ), requirements ) );
    }
    else if (type == "PelicanServerClient" ) {
        _clients.append( new PelicanServerClient( configuration( type, name ), requirements ) );
    }
    else {
        throw QString("Unknown data client type: ") + type;
    }
    return _clients.last();
}

} // namespace pelican

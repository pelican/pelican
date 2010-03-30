#include "AbstractStreamDataClient.h"


#include "utility/memCheck.h"

namespace pelican {
class ConfigNode;
class DataTypes;


/**
 *@details AbstractStreamDataClient 
 */
AbstractStreamDataClient::AbstractStreamDataClient(const ConfigNode& config, const DataTypes& types)
    : AbstractDataClient(config, types)
{
    setIP_Address(config.getOption("server","host"));
    QString port = config.getOption("server","port");
    if( port != "" )
        setPort(port.toUInt());
}

/**
 *@details
 */
AbstractStreamDataClient::~AbstractStreamDataClient()
{
}

void AbstractStreamDataClient::setPort(unsigned int port)
{
    _port = port;
}

void AbstractStreamDataClient::setIP_Address(const QString& ipaddress)
{
    _server = ipaddress;
}

QHash<QString, DataBlob*> AbstractStreamDataClient::getData(QHash<QString, DataBlob*>& dataHash)
{
    QHash<QString, DataBlob*> validData;
    QSet<QString> reqs = _requireSet;
    if( ! reqs.subtract(QSet<QString>::fromList(dataHash.keys())).isEmpty() )
        throw(QString("AbstractStreamDataClient::getData() data hash does not "
                "contain objects for all possible requests"));
    return validData;
}


} // namespace pelican

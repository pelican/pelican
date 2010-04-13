#include "TestDataClient.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 * @details TestDataClient
 */
TestDataClient::TestDataClient(const ConfigNode& config, const DataTypes& types)
    : AbstractDataClient(config, types)
{
}

/**
 * @details
 */
TestDataClient::~TestDataClient()
{
}

/**
 * @details
 * Implements the getData() pure virtual method.
 * The hash of data returned will match all the data types set in the constructor.
 */
QHash<QString, DataBlob*> TestDataClient::getData(QHash<QString, DataBlob*>&)
{
    QHash<QString, DataBlob*> hash;
    foreach (DataRequirements req, dataRequirements()) {
        foreach (QString type, req.allData()) {
            hash.insert(type, 0);
        }
    }
    return hash;
}

} // namespace pelican

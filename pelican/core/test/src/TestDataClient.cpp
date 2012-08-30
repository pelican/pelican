#include "TestDataClient.h"

namespace pelican {
namespace test {


/**
 * @details TestDataClient
 */
TestDataClient::TestDataClient(const ConfigNode& config, const DataSpec& spec) :
    AbstractDataClient(config, DataTypes(), 0)
{
    _dataSpec = spec;
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
    foreach(const DataSpec& req, dataRequirements()) {
        foreach(const QString& type, req.allData()) {
            hash.insert(type, 0);
        }
    }
    return hash;
}

const DataSpec& TestDataClient::dataSpec() const {
    return _dataSpec;
}

} // namespace test
} // namespace pelican

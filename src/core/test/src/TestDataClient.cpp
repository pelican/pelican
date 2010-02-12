#include "TestDataClient.h"
#include "data/DataRequirements.h"
#include "data/DataBlob.h"
#include <QDomElement>
#include <QtGlobal>

#include "utility/memCheck.h"

namespace pelican {


// class TestDataClient 
TestDataClient::TestDataClient()
    : AbstractDataClient(QDomElement(), NULL)
{
}

TestDataClient::~TestDataClient()
{
    foreach (DataBlob* blob, _dataHash.values()) {
        delete blob;
    }
}

/**
 * @details
 * Implementation of getData for the TestDataClient.
 * Returns the requested subset of data.
 */
QHash<QString, DataBlob*>& TestDataClient::getData(const DataRequirements& requirements)
{
    foreach (QString key, _subset) {
        if (!_dataHash.contains(key)) {
            _dataHash.insert(key, new DataBlob);
        }
    }
    return _dataHash;
}

/**
 * @details
 * Sets the subset of the data to return by getData().
 */
void TestDataClient::setSubset(QSet<QString> set)
{
    _subset = set;
}

} // namespace pelican

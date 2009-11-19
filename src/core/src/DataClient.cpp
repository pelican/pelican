#include "DataClient.h"
#include "DataRequirements.h"


namespace pelican {


// class DataClient 
DataClient::DataClient()
{
}

DataClient::~DataClient()
{
}

QHash<QString, DataBlob*> DataClient::getData(const DataRequirements&)
{
    QHash<QString, DataBlob*> dataHash;
    return dataHash;
}

} // namespace pelican

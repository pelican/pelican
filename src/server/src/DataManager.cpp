#include "DataManager.h"

#include "Data.h"
#include "utility/memCheck.h"

namespace pelican {


// class DataManager 
DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

Data DataManager::getNextStreamData(const QString& type)
{
    return Data(0,0);
}

Data DataManager::getServiceData(const QString& data)
{
    return Data(0,0);
}

} // namespace pelican

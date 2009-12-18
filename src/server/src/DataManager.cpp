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

QList<QString> DataManager::streams() const 
{
    return _streams.keys();
}

QList<QString> DataManager::serviceData() const 
{
    return _service.keys();
}

LockedData DataManager::getNext(const QString& type)
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    return _streams[type].getNext();
}

LockedData DataManager::getServiceData(const QString& type, const QString& version)
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    return _service[type].getData(version);
}

} // namespace pelican

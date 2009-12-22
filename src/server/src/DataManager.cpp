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

const DataRequirements& DataManager::dataSpec() const 
{
    return _specs;
}

void DataManager::streamDataBuffer(const QString& name, StreamDataBuffer* buffer)
{
    _specs.setStreamData(name);
    _streams[name]=buffer;
}

void DataManager::serviceDataBuffer(const QString& name, ServiceDataBuffer* buffer )
{
    _specs.setServiceData(name);
    _service[name]=buffer;
}

LockedData DataManager::getNext(const QString& type)
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    return _streams[type]->getNext();
}

LockedData DataManager::getServiceData(const QString& type, const QString& version)
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    return _service[type]->getData(version);
}

} // namespace pelican

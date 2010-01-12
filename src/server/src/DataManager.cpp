#include "DataManager.h"

#include "Data.h"
#include "StreamData.h"
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
    buffer->setDataManager(this);
    _streams[name]=buffer;
}

void DataManager::serviceDataBuffer(const QString& name, ServiceDataBuffer* buffer )
{
    _specs.setServiceData(name);
    _service[name]=buffer;
}

LockedData DataManager::getNext(const QString& type, const QSet<QString>& associateData )
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    LockedData d = getNext(type);
    if( d.isValid() ) {
        // check it contains valid associateData
        StreamData* sd = static_cast<StreamData*>(d.data());
        QSet<QString> tst = sd->associateDataTypes();
        if(! ( associateData - tst ).isEmpty() ) {
            return LockedData(0);
        }
    }
    return d;
}

LockedData DataManager::getNext(const QString& type)
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    LockedData ld(type,0);
    _streams[type]->getNext(ld);
    return ld;
}

LockedData DataManager::getServiceData(const QString& type, const QString& version)
{
    // we make the assumption that this method
    // will not be called with an invalid type
    // No checking in order to speed things up
    LockedData ld(type,0);
    _service[type]->getData(ld, version);
    return ld;
}

void DataManager::associateServiceData(StreamData* data)
{
    QHashIterator<QString, ServiceDataBuffer*> i(_service);
    while (i.hasNext()) {
        i.next();
        LockedData d(i.key());
        i.value()->getCurrent(d);
        if( d.isValid() ) 
            data->addAssociatedData(d);
    }
}

} // namespace pelican

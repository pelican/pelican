#include "DataManager.h"

#include "LockableData.h"
#include "LockableStreamData.h"
#include "comms/StreamData.h"
#include "WritableData.h"
#include "utility/memCheck.h"

namespace pelican {


// class DataManager 
DataManager::DataManager()
{
}

DataManager::~DataManager()
{
    foreach(StreamDataBuffer* s, _streams)
    {
        delete s;
    }
    foreach(ServiceDataBuffer* s, _service)
    {
        delete s;
    }
}

const DataRequirements& DataManager::dataSpec() const 
{
    return _specs;
}

WritableData DataManager::getWritableData( const QString& type, size_t size)
{
    if( _streams.contains(type) )
        return _streams[type]->getWritable(size);
    if( _service.contains(type) )
        return _service[type]->getWritable(size);
    return WritableData(0);
}

StreamDataBuffer* DataManager::getStreamBuffer(const QString& type)
{
    if( _streams.contains(type) )
    {
        streamDataBuffer(type, new StreamDataBuffer(type) );
    }
    return _streams[type];
}

ServiceDataBuffer* DataManager::getServiceBuffer(const QString& type)
{
    if( _service.contains(type) )
    {
        serviceDataBuffer(type, new ServiceDataBuffer(type) );
    }
    return _service[type];
}

void DataManager::streamDataBuffer( const QString& name, StreamDataBuffer* buffer)
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
        LockableStreamData* sd = static_cast<LockableStreamData*>(d.data());
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
    LockedData ld(type,0);
    if( _service.contains(type) ) 
        _service[type]->getData(ld, version);
    return ld;
}

void DataManager::associateServiceData(LockableStreamData* data)
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

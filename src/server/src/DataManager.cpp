#include "DataManager.h"

#include "LockableData.h"
#include "LockableStreamData.h"
#include "comms/StreamData.h"
#include "WritableData.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * DataManager destructor.
 *
 * This deletes all the stream and service data buffers held by
 * the data manager.
 */
DataManager::~DataManager()
{
    foreach(StreamDataBuffer* s, _streams)
        delete s;
    foreach(ServiceDataBuffer* s, _service)
        delete s;
}

/**
 * @details
 *
 */
WritableData DataManager::getWritableData(const QString& type, size_t size)
{
    if ( _streams.contains(type) )
        return _streams[type]->getWritable(size);
    if ( _service.contains(type) )
        return _service[type]->getWritable(size);
    return WritableData(0);
}

/**
 * @details
 * This method sets up a stream buffer for the specified data type
 * if it does not already exist.
 *
 * @param[in] type The data type held by the buffer.
 */
StreamDataBuffer* DataManager::getStreamBuffer(const QString& type)
{
    if ( ! _streams.contains(type) )
        streamDataBuffer( type, new StreamDataBuffer(type) );
    return _streams[type];
}

/**
 * @details
 * This method sets up a service buffer for the specified data type
 * if it does not already exist.
 *
 * @param[in] type The data type held by the buffer.
 */
ServiceDataBuffer* DataManager::getServiceBuffer(const QString& type)
{
    if ( ! _service.contains(type) )
        serviceDataBuffer( type, new ServiceDataBuffer(type) );
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

/**
 * @details
 *
 * We make the assumption that this method will not be called with an
 * invalid type. No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type, const QSet<QString>& associateData )
{
    LockedData lockedData = getNext(type);
    if( lockedData.isValid() ) {
        // check it contains valid associateData
        LockableStreamData* streamData =
                static_cast<LockableStreamData*>(lockedData.object());
        QSet<QString> tst = streamData->associateDataTypes();
        if(! ( associateData - tst ).isEmpty() ) {
            return LockedData(0);
        }
    }
    return lockedData;
}

/**
 * @details
 *
 * We make the assumption that this method will not be called with an
 * invalid type. No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type)
{
    LockedData lockedData(type, 0);
    _streams[type]->getNext(lockedData);
    return lockedData;
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

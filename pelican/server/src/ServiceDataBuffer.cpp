#include "pelican/server/ServiceDataBuffer.h"

#include "pelican/server/LockedData.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/LockableData.h"

#include <QMutexLocker>
#include <stdlib.h>

#include "pelican/utility/memCheck.h"

namespace pelican {


// class ServiceDataBuffer
ServiceDataBuffer::ServiceDataBuffer(const QString& type)
    : DataBuffer(type)
{
    _current = "";
    _newData = 0;

    _max = 10000; //TODO make configurable
    _maxChunkSize = _max;
    _space = _max;
}

ServiceDataBuffer::~ServiceDataBuffer()
{
    delete _newData;
    foreach(LockableData* data, _data ) {
        delete data;
    }
}

void ServiceDataBuffer::getData(LockedData& ld, const QString& version)
{
    QMutexLocker lock(&_mutex);
    ld.setData(_data.value(version));
}

void ServiceDataBuffer::getCurrent(LockedData& ld)
{
    QMutexLocker lock(&_mutex);
    ld.setData( _data.value(_current) );
}

WritableData ServiceDataBuffer::getWritable(size_t size)
{
    if( ! _newData ) {
        QMutexLocker lock(&_mutex);
        for (int i = 0; i < _expiredData.size(); ++i) {
            LockableData* lockableData = _expiredData[i];
            if( lockableData->maxSize() >= size ) {
                // We found one, ensure we remove it from the
                // active data queue.
                _data.remove(lockableData->id());
                _expiredData.removeAt(i);
                return lockableData;
            }
        }
        // create a new data Object if we have enough space
        if(size <= _space && size <= _maxChunkSize )
        {
            void* d = malloc(size);
            if( d ) {
                _space -= size;
                _newData = new LockableData(_type,d, size);
                Q_ASSERT(connect( _newData, SIGNAL(unlockedWrite()), this, SLOT(activateData() ) ));
                Q_ASSERT(connect( _newData, SIGNAL(unlocked()), this, SLOT(deactivateData() ) ));
                return WritableData( _newData );
            }
        }
    }
    return WritableData(0); // no free containers so we return an invalid
}

void ServiceDataBuffer::activateData()
{
    activateData( static_cast<LockableData*>( sender() ) );
}

void ServiceDataBuffer::deactivateData()
{
    deactivateData( static_cast<LockableData*>( sender() ) );
}

void ServiceDataBuffer::deactivateData(LockableData* data)
{
    // We cannot delete the data immediately there is no
    // longer any stream data referencing the data as a client
    // may still ask for the data.
    // If it is not the current service data (which will be associated with
    // any incoming stream data), we mark it as available for removal in the event of
    // the buffer being full.
   if( data->id() != _current && ! _expiredData.contains(data) )
        _expiredData.append(data);
}

void ServiceDataBuffer::activateData(LockableData* data)
{
    QMutexLocker lock(&_mutex);
    if( data->isValid() )
    {
        // determine the version id for this object
        QString id = data->id();
        if( id == "" ) {
            // generate a unique id if not already set
            id.setNum(++_id);
            data->setId(id);
        }

        // move from pending into the active data hash
        _data.insert(id,data);
        _newData = 0;

        // set the current pointer to this one
        _current = id;
    }
}

} // namespace pelican

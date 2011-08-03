#include "pelican/server/LockableStreamData.h"
#include "pelican/server/LockableServiceData.h"
#include "pelican/server/LockedData.h"
#include "pelican/comms/DataChunk.h"
#include "pelican/comms/StreamData.h"

#include <iostream>

namespace pelican {

/**
 * @details
 * LockableStreamData constructor.
 */
LockableStreamData::LockableStreamData(const QString& name, void* memory,
        size_t size, QObject* parent) : AbstractLockableData(size, parent)
{
    _data.reset( new StreamData(name, memory, size) );
    _served = false;
}

LockableStreamData::~LockableStreamData()
{
}

void LockableStreamData::addAssociatedData(const LockedData& data)
{
    _serviceDataTypes.insert(data.name());
    _serviceData.append(data);
    LockableServiceData* lockable = static_cast<LockableServiceData*>(data.object());
    boost::shared_ptr<DataChunk> dataPtr( lockable->data() );
    streamData()->addAssociatedData( dataPtr );
}

bool LockableStreamData::isValid() const
{
    return isValid( _serviceDataTypes );
    /*
    bool rv = Data::isValid();
    foreach(LockedData data, _serviceData )
    {
        rv = rv && data.isValid();
    }
    return rv;
    */
}

bool LockableStreamData::isValid(const QSet<QString>& associates) const
{
    Q_ASSERT( data() != 0 );
    bool rv = data()->isValid();
    foreach(const QString& assoc, associates )
    {
        foreach(LockedData data, _serviceData )
        {
            if( data.name() == assoc )
                rv = rv && data.isValid();
        }
    }
    return rv;
}

void LockableStreamData::reset( size_t size )
{
    _serviceData.clear();
    _serviceDataTypes.clear();
    streamData()->reset( size );
}

StreamData* LockableStreamData::streamData() const {
    return static_cast<StreamData*>(_data.get());
}

} // namespace pelican

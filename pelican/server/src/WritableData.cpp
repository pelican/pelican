#include <string>
#include "pelican/comms/Data.h"
#include "pelican/server/WritableData.h"
#include "pelican/server/AbstractLockableData.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

// class WritableData
WritableData::WritableData(AbstractLockableData* data)
    : _data(data)
{
    if( _data )
        _data->writeLock();
}

WritableData::~WritableData()
{
    if( _data )
        _data->writeUnlock();
}

void WritableData::write(const void* buf, size_t size, size_t offset )
{
//  Q_ASSERT( size + offset <= _data->data()->size() );

    if ( size + offset > _data->data()->size() )
        throw QString("ARRRG!");

    void* mem = (char*)(_data->data()->operator*()) + offset;
    memcpy( mem, buf , size);
}

} // namespace pelican

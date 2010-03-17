#include <string>
#include "comms/Data.h"
#include "WritableData.h"
#include "LockableData.h"

#include "utility/memCheck.h"

namespace pelican {

// class WritableData 
WritableData::WritableData(LockableData* data)
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

LockableData* WritableData::data()
{
    return _data;
}

void WritableData::write(const void* buf, size_t size, size_t offset )
{
    Q_ASSERT( size + offset <= _data->data()->size() );
    std::memcpy(_data->data()->operator*(), buf + offset , size);
}

bool WritableData::isValid() const
{
    if( _data )
        return  _data->isValid();
    return false;
}

} // namespace pelican

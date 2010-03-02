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

bool WritableData::isValid() const
{
    if( _data )
        return  _data->isValid();
    return false;
}

} // namespace pelican

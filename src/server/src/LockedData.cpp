#include "LockedData.h"
#include "Data.h"


#include "utility/memCheck.h"

namespace pelican {


// class LockedData 
LockedData::LockedData( Data* data )
    : _data(data)
{
    _data->lock();
}

LockedData::~LockedData()
{
    _data->unlock();
}

bool LockedData::isValid() const
{
    _data->isValid();
}

} // namespace pelican

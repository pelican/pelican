#include "WritableData.h"
#include "Data.h"

#include "utility/memCheck.h"

namespace pelican {

// class WritableData 
WritableData::WritableData(Data* data)
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

Data* WritableData::data()
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

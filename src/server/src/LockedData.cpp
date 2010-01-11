#include "LockedData.h"
#include "Data.h"

#include "utility/memCheck.h"

namespace pelican {


// class LockedData 
LockedData::LockedData( Data* data )
{
    addData(data);
}

LockedData::~LockedData()
{
    foreach (Data* data, _data ) {
        data->unlock();
    }
}

LockedData::LockedData( const LockedData& data )
{
    addData(data);
}

void LockedData::addData(const LockedData& data)
{
    foreach (Data* d, data._data ) {
        addData(d);
    }
}

void LockedData::addData(Data* data)
{
    if( data) {
        data->lock();
        _data.append(data);
    }
}

bool LockedData::isValid() const
{
    bool rv=( _data.isEmpty())?false:true;
    foreach (Data* data, _data ) {
        rv = rv && data->isValid();
    }
    return rv;
}

size_t LockedData::size() const
{
    size_t s = 0;
    foreach( Data* data , _data )
    {
        if( data->isValid() )
            s += data->size();
    }
    return s;
}

} // namespace pelican

#include "LockedData.h"
#include "Data.h"

#include "utility/memCheck.h"

namespace pelican {


// class LockedData 
LockedData::LockedData( const QString& name, Data* data )
    : _name(name)
{
    _data = 0;
    setData(data);
}

LockedData::~LockedData()
{
    if( _data ) 
        _data->unlock();
}

LockedData::LockedData( const LockedData& data )
{
    _name = data._name;
    _data = 0;
    setData(data._data);
}

void LockedData::setData(Data* data)
{
    if( data) {
        _data=data;
        data->lock();
    }
}

bool LockedData::isValid() const
{
    //bool rv=( _data.isEmpty())?false:true;
    //foreach (Data* data, _data ) {
    //    rv = rv && data->isValid();
    //}
    //return rv;
    bool rv = false;
    if ( _data ) rv = _data->isValid();
    return rv;
}

size_t LockedData::size() const
{
    size_t s = 0;
    if( _data && _data->isValid() )
           s = _data->size();
    return s;
}

} // namespace pelican

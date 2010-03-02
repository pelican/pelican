#include "LockedData.h"
#include "LockableData.h"
#include "comms/Data.h"
#include "utility/memCheck.h"

namespace pelican {


// class LockedData 
LockedData::LockedData( const QString& name, LockableData* data )
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

void LockedData::setData(LockableData* data)
{
    if( data ) {
        _data=data;
        data->data()->setName(_name);
        data->lock();
    }
}

LockableData* LockedData::data() const
{
    return _data; 
}

bool LockedData::isValid() const
{
    //bool rv=( _data.isEmpty())?false:true;
    //foreach (Data* data, _data ) {
    //    rv = rv && data->isValid();
    //}
    //return rv;
    bool rv = false;
    if ( _data && _data->data() )
        rv = _data->data()->isValid();
    return rv;
}

size_t LockedData::size() const
{
    size_t s = 0;
    if( isValid() )
           s = _data->data()->size();
    return s;
}

} // namespace pelican

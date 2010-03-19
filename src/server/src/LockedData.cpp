#include "LockedData.h"
#include "AbstractLockable.h"
#include "utility/memCheck.h"

namespace pelican {


// class LockedData 
LockedData::LockedData( const QString& name, AbstractLockable* data )
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

void LockedData::setData(AbstractLockable* data)
{
    if( data ) {
        _data=data;
        data->lock();
    }
}

AbstractLockable* LockedData::object() const
{
    return _data; 
}

bool LockedData::isValid() const
{
    bool rv = false;
    if ( _data  )
        rv = _data->isValid();
    return rv;
}

} // namespace pelican

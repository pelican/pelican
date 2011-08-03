#include "pelican/server/LockedData.h"

namespace pelican {

/**
 * @details
 * Constructs a new LockedData object.
 */
LockedData::LockedData( const QString& name, AbstractLockable* data )
    : _name(name)
{
    _data = 0;
    setData(data);
}

/**
 * @details
 * Destroys the LockedData object, decreasing the lock count on the
 * data it contains.
 */
LockedData::~LockedData()
{
    if( _data )
        _data->unlock();
}

/**
 * @details
 * Creates a copy of a LockedData object.
 */
LockedData::LockedData( const LockedData& data )
{
    _name = data._name;
    _data = 0;
    setData(data._data);
}

/**
 * @details
 * Sets the data object and locks it.
 */
void LockedData::setData(AbstractLockable* data)
{
    if( data ) {
        _data = data;
        data->lock();
    }
}

} // namespace pelican

#include "Data.h"
#include <QMutexLocker>
#include <iostream>
#include "utility/memCheck.h"

namespace pelican {


// class Data 
Data::Data(void* data, size_t size, QObject* parent)
    : QObject(parent), _data(data), _size(size), _lock(0), _wlock(0)
{
}

Data::~Data()
{
}

void Data::writeLock()
{
    QMutexLocker locker(&_mutex);
    ++_wlock;
}

void Data::writeUnlock()
{
    QMutexLocker locker(&_mutex);
    --_wlock;
    if( ! _wlock ) {
        emit unlockedWrite();
    }
}

bool Data::isLocked() const
{
    return (bool)(_lock || _wlock);
}

void Data::lock()
{
    QMutexLocker locker(&_mutex);
    ++_lock;
}

void Data::unlock()
{
    QMutexLocker locker(&_mutex);
    --_lock;
    if( _lock <= 0 ) {
       emit unlocked();
    }
}

bool Data::isValid() const 
{
    return !( _data == 0 || _size == 0);
}

void* Data::operator*()
{
    return _data;
}

void Data::setSize(size_t s)
{
    _size = s;
}

size_t Data::size() const
{
    return _size;
}

} // namespace pelican

#include <QMutexLocker>
#include <iostream>
#include "LockableData.h"
#include "comms/Data.h"
#include "utility/memCheck.h"

namespace pelican {

// class LockableData 
LockableData::LockableData(const QString& name, void* data, size_t size, QObject* parent)
    : QObject(parent), _lock(0), _wlock(0)
{
    _data = new Data(name, data, size);
}

LockableData::~LockableData()
{
    delete _data;
}

void LockableData::writeLock()
{
    QMutexLocker locker(&_mutex);
    ++_wlock;
}

void LockableData::writeUnlock()
{
    QMutexLocker locker(&_mutex);
    --_wlock;
    if( ! _wlock ) {
        emit unlockedWrite();
    }
}

bool LockableData::isLocked() const
{
    return (bool)(_lock || _wlock);
}

void LockableData::lock()
{
    QMutexLocker locker(&_mutex);
    ++_lock;
}

void LockableData::unlock()
{
    QMutexLocker locker(&_mutex);
    --_lock;
    if( _lock <= 0 ) {
       emit unlocked();
    }
}

bool LockableData::isValid() const
{
    return _data->isValid();
}

Data* LockableData::data() const
{
    return _data;
}

void* LockableData::memoryLocation()
{
    return _data->operator*();
}

void LockableData::setId(const QString& id) 
{
    _data->setId(id); 
}

QString LockableData::id() const { 
    return _data->id(); 
}

//void LockableData::setSize(size_t s)
//{
//    _data->setSize(s);
//}

//size_t LockableData::size() const
//{
//    return _data->size();
//}

} // namespace pelican

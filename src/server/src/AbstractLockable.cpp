#include "AbstractLockable.h"
#include <QMutexLocker>


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details AbstractLockable 
 */
AbstractLockable::AbstractLockable(QObject* parent)
    : QObject(parent), _lock(0), _wlock(0)
{
}

/**
 *@details
 */
AbstractLockable::~AbstractLockable()
{
}

void AbstractLockable::writeLock()
{
    QMutexLocker locker(&_mutex);
    ++_wlock;
}

void AbstractLockable::writeUnlock()
{
    QMutexLocker locker(&_mutex);
    --_wlock;
    if( ! _wlock ) {
        emit unlockedWrite();
    }
}

bool AbstractLockable::isLocked() const
{
    return (bool)(_lock || _wlock);
}

void AbstractLockable::lock()
{
    QMutexLocker locker(&_mutex);
    ++_lock;
}

void AbstractLockable::unlock()
{
    QMutexLocker locker(&_mutex);
    --_lock;
    if( _lock <= 0 ) {
       emit unlocked();
    }
}
} // namespace pelican

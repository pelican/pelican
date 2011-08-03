#include "pelican/server/AbstractLockable.h"

namespace pelican {

/**
 * @details
 * Decreases the write lock counter, and emits the unlockedWrite signal when
 * count returns to 0.
 */
void AbstractLockable::writeUnlock()
{
    QMutexLocker locker(&_mutex);
    --_wlock;
    if ( ! _wlock )
        emit unlockedWrite();
}

/**
 * @details
 * Decreases the lock counter, and emits the unlocked signal when
 * count returns to 0.
 */
void AbstractLockable::unlock()
{
    QMutexLocker locker(&_mutex);
    --_lock;
    if ( _lock <= 0 )
       emit unlocked();
}

} // namespace pelican

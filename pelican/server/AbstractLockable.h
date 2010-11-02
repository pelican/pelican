#ifndef ABSTRACT_LOCKABLE_H
#define ABSTRACT_LOCKABLE_H

/**
 * @file AbstractLockable.h
 */

#include <cstring>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>

namespace pelican {

/**
 * @ingroup c_server
 *
 * @class AbstractLockable
 *
 * @brief
 * Wrapper for objects to lock/unlock with a LockedData.
 *
 * @details
 * Two types of lock are supported, write and read.
 * This class emits signals when unlocked.
 *
 * Inherited by the AbstractLockableData class.
 */
class AbstractLockable : public QObject
{
    Q_OBJECT

    private:
        int _lock;  ///< Counts the number of times the lock has been applied.
        int _wlock; ///< Counts the number of times the write lock has been applied.

    protected:
        QMutex _mutex;

    public:
        /// AbstractLockable constructor.
        AbstractLockable(QObject* parent = 0)
        : QObject(parent), _lock(0), _wlock(0) {}

        /// AbstractLockable destructor.
        ~AbstractLockable() {}

        /// Returns true if there is an active lock on the data.
        bool isLocked() const {return bool(_lock || _wlock);}

        /// Returns true if the object is initialised and ready for use.
        virtual bool isValid() const = 0;

        /// Marks the data as locked (increases count on unlimited semaphore).
        void lock() {QMutexLocker locker(&_mutex); ++_lock;}

        /// Marks the data as unlocked (decreases count on semaphore).
        void unlock();

        /// Marks the data as locked (increases count on unlimited semaphore).
        void writeLock() {QMutexLocker locker(&_mutex); ++_wlock;}

        /// Marks the data as unlocked (decreases count on semaphore).
        void writeUnlock();

    private:
        /// Disallow the copy constructor.
        AbstractLockable(const AbstractLockable&);

    signals:
        /// Signal emitted when the lock count reaches zero.
        void unlocked();

        /// Signal emitted when the write lock count reaches zero.
        void unlockedWrite();
};

} // namespace pelican

#endif // ABSTRACT_LOCKABLE_H

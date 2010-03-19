#ifndef ABSTRACTLOCKABLE_H
#define ABSTRACTLOCKABLE_H
#include <cstring>
#include <QObject>
#include <QMutex>

/**
 * @file AbstractLockable.h
 */

namespace pelican {

/**
 * @class AbstractLockable
 *  
 * @brief
 *   Wrapper for objects to Lock/unlock with a LockedData
 *   Emits signals when unlocked
 * @details
 *   Two types of lock are supported, write and read
 * 
 */

class AbstractLockable : public QObject
{
    Q_OBJECT

    public:
        AbstractLockable( QObject* parent=0 );
        ~AbstractLockable();

        /// returns true if there is an active lock on the data
        bool isLocked() const;

        /// marks the data as locked (increases count on unlimited semaphore)
        void lock();

        /// marks the data as unlocked (decreases count on semaphore)
        //  emits the unlocked signal when count returns to 0
        void unlock();

        /// marks the data as locked (increases count on unlimited semaphore)
        void writeLock();

        /// marks the data as unlocked (decreases count on semaphore)
        //  emits the unlockedWrite signal when count returns to 0
        void writeUnlock();

        /// returns true if the object is correctly initialised
        //  and can be used
        virtual bool isValid() const = 0;

    private:
        AbstractLockable(const AbstractLockable&); // disallow the copy constructor

    signals:
        void unlocked();
        void unlockedWrite();

    protected:
        QMutex _mutex;

    private:
        int _lock;
        int _wlock;
};

} // namespace pelican
#endif // ABSTRACTLOCKABLE_H 

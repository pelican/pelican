#ifndef LOCKABLEDATA_H
#define LOCKABLEDATA_H

#include <cstring>
#include <QMutex>
#include <QObject>

/**
 * @file LockableData.h
 */

namespace pelican {

class Data;

/**
 * @class LockableData
 *  
 * @brief
 *    Primary interface to access Chunks of data in the server
 * @details
 *    This class takes care of locking etc.
 */

class LockableData : public QObject
{

    Q_OBJECT

    public:
        LockableData(const QString& name="", void* data=0, size_t size=0, QObject* parent=0);
        ~LockableData();
        /// return the size of the stored data
        //size_t size() const;
        /// sets the size of the stored data
        //void setSize(size_t);

        /// returns a pointer to the beginning of the memory block
        void* memoryLocation();

        /// returns the Data object
        Data* data() const;

        /// returns true if there isany valid data in the object
        bool isValid() const;

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

        /// return the data id 
        QString id() const;

        /// sets the id 
        void setId(const QString& id);

    signals:
        void unlocked();
        void unlockedWrite();

    protected:
        QMutex _mutex;

    private:
        LockableData(const LockableData&); // disallow the copy constructor

    private:
        //QString _id;
        Data* _data;
        int _lock;
        int _wlock;
};

} // namespace pelican
#endif // LOCKABLEDATA_H 

/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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

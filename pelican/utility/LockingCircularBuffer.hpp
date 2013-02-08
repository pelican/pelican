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

#ifndef LOCKINGCIRCULARBUFFER_H
#define LOCKINGCIRCULARBUFFER_H
#include "CircularBufferIterator.hpp"
#include <QtCore/QList>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>


/**
 * @file LockingCircularBuffer.h
 */

namespace pelican {

/**
 * @class LockingCircularBuffer
 *
 * @brief
 *   A convenience class to manage two pointers on the same circular buffer
 *   Two pointers are defined, lthe lock pointer and the current pointer.
 *   The current pointer will not be allowed to move past the position
 *   of the lock pointer.
 * @details
 *
 */

template<typename T>
class LockingCircularBuffer
{
    public:
        LockingCircularBuffer( QList<T>* dataBuffer ) {
             _lock = 0;
             _current = new CircularBufferIterator<T>(dataBuffer);
        }
        ~LockingCircularBuffer() { delete _lock; delete _current; };

        /// return the next free item, this will BLOCK until there are free
        //  elements.
        T next() {
             QMutexLocker lock(&_mutex);
             const T& next = _current->peekNext();
             while( _lock && *(*_lock) == next ) {
                _waitCondition.wait(&_mutex);
             }
             return _current->next();
        };

        /// move the lock pointer to the next position
        //  and return a CircularBufferIterator starting at the lock position
        const CircularBufferIterator<T>& shiftLock() {
             QMutexLocker lock(&_mutex);
             if( ! _lock ) {
                 _lock = new CircularBufferIterator<T>(_current->buffer());
             }
             _lock->next();
             _waitCondition.wakeOne();
             return *_lock;
        }

        /// return the value pointed to by the current lock
        T& current() const { return *(*_current); }

        /// returns a pointer to the underlying data buffer
        QList<T>* rawBuffer() { return _current->buffer(); }

    private:
        QWaitCondition _waitCondition;
        QMutex _mutex;
        CircularBufferIterator<T>* _lock;
        CircularBufferIterator<T>* _current;
};

} // namespace pelican
#endif // LOCKINGCIRCULARBUFFER_H
